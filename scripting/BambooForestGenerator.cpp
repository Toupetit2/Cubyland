#include "script_pch.h" 
#include <vector>
#include <cmath>
#include <cstdlib> // For rand()
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

using namespace Engine;

class BambooForestGenerator : public Engine::Scripting::NativeScript {
private:
    // --- CUSTOM WIND SHADER DEFINITION ---
    // const char* BAMBOO_WIND_VERT_SRC = R"(
    //     #version 330 core
    //     layout (location = 0) in vec3 inPos;
    //     layout (location = 1) in vec3 inNormal;
    //     layout (location = 2) in vec2 inUV;

    //     uniform mat4 uModel;
    //     uniform mat4 uView;
    //     uniform mat4 uProjection;

    //     // Wind & Physics Uniforms
    //     uniform float uTime;
    //     uniform vec2 uWindDirection;
    //     uniform float uWindStrength;
    //     uniform float uBambooHeight;

    //     out vec3 fragPosWorld;
    //     out vec3 fragNormalWorld;
    //     out vec2 fragUV;

    //     void main() {
    //         // 1. Calculate height mask (0.0 at bottom, 1.0 at top)
    //         float heightMask = clamp(inPos.y / uBambooHeight, 0.0, 1.0);
            
    //         // 2. Exponential bending (stiff at base, bends heavily at top)
    //         float bendFactor = pow(heightMask, 2.0);
            
    //         // 3. Create phase-varied oscillation based on world position
    //         vec4 worldPos = uModel * vec4(inPos, 1.0);
    //         float windOscillation = sin(uTime * 2.5 + worldPos.x * 0.5 + worldPos.z * 0.5);
            
    //         // 4. Apply displacement
    //         vec3 displacedPos = inPos;
    //         displacedPos.x += uWindDirection.x * windOscillation * uWindStrength * bendFactor;
    //         displacedPos.z += uWindDirection.y * windOscillation * uWindStrength * bendFactor;
    //         displacedPos.y -= abs(windOscillation) * (uWindStrength * 0.25) * bendFactor;

    //         fragPosWorld = (uModel * vec4(displacedPos, 1.0)).xyz;
    //         fragNormalWorld = mat3(transpose(inverse(uModel))) * inNormal;
    //         fragUV = inUV;

    //         gl_Position = uProjection * uView * vec4(fragPosWorld, 1.0);
    //     }
    // )";

    const char* BAMBOO_WIND_VERT_SRC = R"(
        #version 330 core
        layout (location = 0) in vec3 inPos;
        layout (location = 1) in vec3 inNormal;
        layout (location = 2) in vec2 inUV;

        uniform mat4 uModel;
        uniform mat4 uView;
        uniform mat4 uProjection;

        uniform float uTime;
        uniform vec2 uWindDirection;
        uniform float uWindStrength;
        uniform float uBambooHeight;

        // Output WORLD SPACE for the Engine's G-Buffer
        out vec3 fragPosWorld;
        out vec3 fragNormalWorld;
        out vec2 fragUV;

        void main() {
            // 1. EXTRACT PACKED PHYSICS
            float treeBendFactor = inNormal.y;
            vec3 localNormal = normalize(vec3(inNormal.x, 0.0, inNormal.z));

            // 2. WIND PHYSICS
            float heightMask = clamp(inPos.y / uBambooHeight, 0.0, 1.0);
            float bendProfile = pow(heightMask, 2.5); 
            
            vec4 worldPos = uModel * vec4(inPos, 1.0);
            
            float inertia = 1.0 + (treeBendFactor * 0.8); 
            float frequency = 1.0 / inertia; 
            float stiffness = 1.0 + (treeBendFactor * 0.5); 
            
            float t = uTime * frequency * (1.0 + uWindStrength * 0.2); 
            
            float wave1 = sin(t * 1.5 + worldPos.x * 0.8 + worldPos.z * 0.8);
            float wave2 = sin(t * 0.3 + worldPos.x * 0.4 + worldPos.z * 0.4) * 1.2;
            float wave3 = sin(t * 5.0 + worldPos.x * 1.5) * (0.15 / inertia) * heightMask;
            
            float windOscillation = (wave1 + wave2 + wave3) * 0.4 + 0.6;
            
            vec2 totalWindDisp = uWindDirection * windOscillation * (uWindStrength / stiffness) * bendProfile;
            
            vec3 displacedPos = inPos;
            displacedPos.x += totalWindDisp.x;
            displacedPos.z += totalWindDisp.y;
            
            float horizontalDisplacement = length(totalWindDisp);
            displacedPos.y -= (horizontalDisplacement * horizontalDisplacement) * 0.15;

            // 3. TRANSFORM TO WORLD SPACE
            vec4 finalWorldPos = uModel * vec4(displacedPos, 1.0);
            fragPosWorld = finalWorldPos.xyz;
            
            // Transform normal using only the Model matrix
            mat3 normalMatrix = mat3(transpose(inverse(uModel)));
            fragNormalWorld = normalMatrix * localNormal;
            
            fragUV = inUV;

            gl_Position = uProjection * uView * finalWorldPos; 
        }
    )";

    const char* BAMBOO_WIND_FRAG_SRC = R"(
        #version 330 core
        layout (location = 0) out vec4 gPositionMetallic;
        layout (location = 1) out vec4 gNormalRoughness;
        layout (location = 2) out vec4 gAlbedo;

        // IN from Vertex Shader (Now correctly in World Space)
        in vec3 fragPosWorld;
        in vec3 fragNormalWorld;
        in vec2 fragUV;
        
        void main() {
            // Write World Space Position. Metallic = 0.0
            gPositionMetallic = vec4(fragPosWorld, 0.0);
            
            // Write World Space Normal. Roughness = 0.6 
            gNormalRoughness = vec4(normalize(fragNormalWorld), 0.6);
            
            vec3 albedo = vec3(0.35, 0.55, 0.2); 
            
            // Darken the bamboo closer to its roots based on UV height
            albedo *= mix(0.5, 1.2, clamp(fragUV.y / 30.0, 0.0, 1.0)); 

            gAlbedo = vec4(albedo, 1.0);
        }
    )";

    Components::Transform* transform = nullptr;
    Components::MeshRenderer* modelRenderer = nullptr;

    float currentTime = 0.0f;

    // Bamboo Parameters
    float m_Height = 50.0f;
    float m_BaseRadius = 1.0f;
    int m_Segments = 20;
    int m_RingsPerSegment = 5;
    int m_RadialSegments = 12;
    float m_TaperFactor = 0.4f;
    float m_BendFactor = 1.5f;

    // --- BATCHING PARAMETERS ---
    int m_TreesPerChunk = 200;       // How many trees in this single draw call
    float m_ChunkRadius = 50.0f;    // How far they spread out

public:
    void OnCreate() override {
        registry->GetOrAddComponent<Components::Transform>(entityID);
        transform = &registry->GetComponent<Components::Transform>(entityID);
        
        modelRenderer = &registry->GetOrAddComponent<Components::MeshRenderer>(entityID);
        
        if (modelRenderer) {
            GenerateForestChunk(); // Call the batched generator
        }

        // --- Load and Assign Custom Wind Shader (Same as before) ---
        Systems::RenderSystem* rs = engine->GetSystem<Systems::RenderSystem>();
        const std::string shaderName = "bamboo_wind_shader";

        if (rs) {
            if (!rs->HasShader(shaderName)) {
                rs->AddShader(shaderName, BAMBOO_WIND_VERT_SRC, BAMBOO_WIND_FRAG_SRC);
            }
            
            Components::MaterialOverride* material = &registry->GetOrAddComponent<Components::MaterialOverride>(entityID, shaderName);
            
            material->Uniforms["uBambooHeight"] = m_Height;
            material->Uniforms["uWindStrength"] = 1.5f;
            material->Uniforms["uWindDirection"] = std::array<float, 2>{1.0f, 0.3f};

            // REMOVE the Transparent component so it renders in the opaque geometry pass!
            if (registry->HasComponent<Components::Transparent>(entityID)) {
                registry->RemoveComponent<Components::Transparent>(entityID);
            }
        }
    }

    void GenerateForestChunk() {
        auto mesh = std::make_shared<Graphics::Mesh>();

        float segmentHeight = m_Height / m_Segments;
        int totalRings = m_Segments * m_RingsPerSegment + 1;
        int verticesPerTree = totalRings * (m_RadialSegments + 1);

        uint32_t baseVertexIndex = 0; 

        for (int tree = 0; tree < m_TreesPerChunk; tree++) {
            
            // 1. Randomize Position
            float offsetX = ((rand() % 2000) / 1000.0f - 1.0f) * m_ChunkRadius;
            float offsetZ = ((rand() % 2000) / 1000.0f - 1.0f) * m_ChunkRadius;
            
            // 2. Randomize Dimensions & Orientation
            float treeHeight = m_Height * (((rand() % 60) + 70) / 100.0f);   // 70% to 130% height
            float treeRadius = m_BaseRadius * (((rand() % 60) + 70) / 100.0f); // 70% to 130% thickness
            float treeBendFactor = m_BendFactor * (((rand() % 100) / 50.0f));  // 0% to 200% bend amount
            
            // Random Yaw Rotation in radians (0 to 2*PI)
            float treeYaw = (rand() % 360) * (glm::pi<float>() / 180.0f);
            float cosYaw = cos(treeYaw);
            float sinYaw = sin(treeYaw);

            for (int r = 0; r < totalRings; r++) {
                float y = (treeHeight * r) / (totalRings - 1);
                float localY = fmod(y, segmentHeight);
                float t = localY / segmentHeight; 
                
                float heightPercent = y / treeHeight;
                float taper = 1.0f - (m_TaperFactor * heightPercent); 

                float bulge = 1.0f;
                if (t < 0.1f) {
                    bulge = 1.0f + (0.1f - t) * 1.2f; 
                } else if (t > 0.9f) {
                    bulge = 1.0f + (t - 0.9f) * 1.2f; 
                } else {
                    bulge = 0.95f; 
                }

                float currentRadius = treeRadius * taper * bulge;
                
                // The static curve of the bamboo (pre-rotation)
                float localBendX = sin(y * 0.1f) * treeBendFactor * heightPercent; 

                for (int s = 0; s <= m_RadialSegments; s++) {
                    float theta = (2.0f * glm::pi<float>() * s) / m_RadialSegments;
                    
                    // Base local coordinates (before rotating the tree)
                    float px_local = localBendX + cos(theta) * currentRadius;
                    float pz_local = sin(theta) * currentRadius;

                    // 3. Apply Tree Yaw Rotation to the Position
                    float rotatedX = px_local * cosYaw - pz_local * sinYaw;
                    float rotatedZ = px_local * sinYaw + pz_local * cosYaw;

                    // Final World Position (add the chunk offset)
                    float px = offsetX + rotatedX;
                    float py = y;
                    float pz = offsetZ + rotatedZ;

                    // Base local normal (Horizontal only)
                    float nx_local = cos(theta);
                    float nz_local = sin(theta);
                    
                    // 4. Apply Tree Yaw Rotation to the Normal. 
                    // WE DO NOT NORMALIZE THIS YET! We are packing the unique treeBendFactor into the Y-axis.
                    glm::vec3 packedNormal = glm::vec3(
                        nx_local * cosYaw - nz_local * sinYaw,
                        treeBendFactor, // <-- SMUGGLING THE CURVATURE DATA HERE
                        nx_local * sinYaw + nz_local * cosYaw
                    );

                    float u = (float)s / m_RadialSegments;
                    float v = y / treeHeight * m_Segments; 

                    mesh->vertex_data.push_back(px);
                    mesh->vertex_data.push_back(py);
                    mesh->vertex_data.push_back(pz);
                    mesh->vertex_data.push_back(packedNormal.x);
                    mesh->vertex_data.push_back(packedNormal.y); // Carries the bend factor!
                    mesh->vertex_data.push_back(packedNormal.z);
                    mesh->vertex_data.push_back(u);
                    mesh->vertex_data.push_back(v);
                }
            }

            for (int r = 0; r < totalRings - 1; r++) {
                for (int s = 0; s < m_RadialSegments; s++) {
                    uint32_t current = baseVertexIndex + (r * (m_RadialSegments + 1) + s);
                    uint32_t next = current + (m_RadialSegments + 1);

                    mesh->indices.push_back(current);
                    mesh->indices.push_back(next);
                    mesh->indices.push_back(current + 1);

                    mesh->indices.push_back(current + 1);
                    mesh->indices.push_back(next);
                    mesh->indices.push_back(next + 1);
                }
            }

            baseVertexIndex += verticesPerTree;
        }

        mesh->vertexCount = baseVertexIndex;
        mesh->upload();
        
        modelRenderer->active = true;
        modelRenderer->meshes.push_back(std::move(mesh));
    }

    void OnUpdate(float deltaTime) override {
        currentTime += deltaTime;
        if (Components::MaterialOverride* material = &registry->GetComponent<Components::MaterialOverride>(entityID)) {
            material->Uniforms["uTime"] = currentTime;
            material->Uniforms["uViewPos"] = std::array<float, 3>{0.0f, 10.0f, 30.0f}; // Don't forget the view pos!
        }
    }
};

extern "C" __declspec(dllexport) Engine::Scripting::NativeScript* CreateScript() {
    return new BambooForestGenerator();
}
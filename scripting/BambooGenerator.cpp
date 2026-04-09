#include "script_pch.h" 
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

using namespace Engine;

class BambooGenerator : public Engine::Scripting::NativeScript {
private:
    // --- CUSTOM WIND SHADER DEFINITION ---
    const char* BAMBOO_WIND_VERT_SRC = R"(
        #version 330 core
        layout (location = 0) in vec3 inPos;
        layout (location = 1) in vec3 inNormal;
        layout (location = 2) in vec2 inUV;

        uniform mat4 uModel;
        uniform mat4 uView;
        uniform mat4 uProjection;

        // Wind & Physics Uniforms
        uniform float uTime;
        uniform vec2 uWindDirection;
        uniform float uWindStrength;
        uniform float uBambooHeight;

        out vec3 fragPosWorld;
        out vec3 fragNormalWorld;
        out vec2 fragUV;

        void main() {
            // 1. Calculate height mask (0.0 at bottom, 1.0 at top)
            float heightMask = clamp(inPos.y / uBambooHeight, 0.0, 1.0);
            
            // 2. Exponential bending (stiff at base, bends heavily at top)
            float bendFactor = pow(heightMask, 2.0);
            
            // 3. Create phase-varied oscillation based on world position
            vec4 worldPos = uModel * vec4(inPos, 1.0);
            float windOscillation = sin(uTime * 2.5 + worldPos.x * 0.5 + worldPos.z * 0.5);
            
            // 4. Apply displacement
            vec3 displacedPos = inPos;
            displacedPos.x += uWindDirection.x * windOscillation * uWindStrength * bendFactor;
            displacedPos.z += uWindDirection.y * windOscillation * uWindStrength * bendFactor;
            displacedPos.y -= abs(windOscillation) * (uWindStrength * 0.25) * bendFactor;

            fragPosWorld = (uModel * vec4(displacedPos, 1.0)).xyz;
            fragNormalWorld = mat3(transpose(inverse(uModel))) * inNormal;
            fragUV = inUV;

            gl_Position = uProjection * uView * vec4(fragPosWorld, 1.0);
        }
    )";

    const char* BAMBOO_WIND_FRAG_SRC = R"(
        #version 330 core
        out vec4 FragColor;

        in vec3 fragPosWorld;
        in vec3 fragNormalWorld;
        in vec2 fragUV;
        
        uniform vec3 uViewPos;
        uniform vec3 uLightPos;
        uniform vec3 uLightColor;
        uniform float uLightIntensity;
        
        void main() {
            // Base Bamboo Color
            vec3 albedo = vec3(0.4, 0.6, 0.2); 
            
            vec3 normal = normalize(fragNormalWorld);
            vec3 lightDir = normalize(uLightPos - fragPosWorld);
            
            // 1. Ambient
            vec3 ambient = 0.3 * uLightColor; // Base ambient light in the scene
            
            // 2. Diffuse
            float diff = max(dot(normal, lightDir), 0.0);
            vec3 diffuse = diff * uLightColor * uLightIntensity;
            
            // 3. Specular (Shininess)
            vec3 viewDir = normalize(uViewPos - fragPosWorld);
            vec3 halfwayDir = normalize(lightDir + viewDir);
            float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0); // 32.0 makes it look glossier
            vec3 specular = spec * uLightColor * (uLightIntensity * 0.5);
            
            // 4. Final Composite: Note that specular is added AFTER multiplying albedo
            vec3 litColor = (ambient + diffuse) * albedo + specular;
            
            FragColor = vec4(litColor, 1.0);
        }
    )";

    // Component Pointers
    Components::Transform* transform = nullptr;
    Components::MeshRenderer* modelRenderer = nullptr;

    // Bamboo Generation Parameters
    float m_Height = 30.0f;
    float m_BaseRadius = 1.0f;
    int m_Segments = 10;
    int m_RingsPerSegment = 5;
    int m_RadialSegments = 12;
    float m_TaperFactor = 0.4f;
    float m_BendFactor = 1.5f; // Static structural bend

    float currentTime = 0.0f;

public:
    void OnCreate() override {
        registry->GetOrAddComponent<Components::Transform>(entityID);
        transform = &registry->GetComponent<Components::Transform>(entityID);
        if (!transform) {
            TerminalInstance->error("Transform not found...");
        }

        modelRenderer = &registry->GetOrAddComponent<Components::MeshRenderer>(entityID);
        if (!modelRenderer) {
            TerminalInstance->error("ModelRenderer not found...");
        } else {
            GenerateBambooGeometry();
        }

        // --- Load and Assign Custom Wind Shader ---
        Systems::RenderSystem* rs = engine->GetSystem<Systems::RenderSystem>();
        const std::string shaderName = "bamboo_wind_shader";

        if (rs) {
            if (!rs->HasShader(shaderName)) {
                TerminalInstance->info("Loading custom shader: " + shaderName);
                rs->AddShader(shaderName, BAMBOO_WIND_VERT_SRC, BAMBOO_WIND_FRAG_SRC);
            }
            
            // Assign the MaterialOverride with the custom shader
            Components::MaterialOverride* material = &registry->GetOrAddComponent<Components::MaterialOverride>(entityID, shaderName);
            
            // Set static shader uniforms
            material->Uniforms["uBambooHeight"] = m_Height;
            material->Uniforms["uWindStrength"] = 1.5f;
            material->Uniforms["uWindDirection"] = std::array<float, 2>{1.0f, 0.3f};
            
            // Lighting Uniforms
            material->Uniforms["uLightPos"] = std::array<float, 3>{10.0f, 20.0f, 10.0f};
            material->Uniforms["uLightColor"] = std::array<float, 3>{1.0f, 1.0f, 0.9f};
            material->Uniforms["uLightIntensity"] = 1.2f;
            
        } else {
            TerminalInstance->error("Could not get RenderSystem to load custom shader.");
        }

        if (transform && modelRenderer) {
            TerminalInstance->success("BambooGenerator Init successfully...");
        }
    }

    void GenerateBambooGeometry() {
        auto mesh = std::make_shared<Graphics::Mesh>();

        float segmentHeight = m_Height / m_Segments;
        int totalRings = m_Segments * m_RingsPerSegment + 1;

        // Generate Vertices
        for (int r = 0; r < totalRings; r++) {
            float y = (m_Height * r) / (totalRings - 1);
            float localY = fmod(y, segmentHeight);
            float t = localY / segmentHeight; 
            
            float heightPercent = y / m_Height;
            float taper = 1.0f - (m_TaperFactor * heightPercent); 

            float bulge = 1.0f;
            if (t < 0.1f) {
                bulge = 1.0f + (0.1f - t) * 1.2f; 
            } else if (t > 0.9f) {
                bulge = 1.0f + (t - 0.9f) * 1.2f; 
            } else {
                bulge = 0.95f; 
            }

            float currentRadius = m_BaseRadius * taper * bulge;
            float xOffset = sin(y * 0.1f) * m_BendFactor * heightPercent; 

            for (int s = 0; s <= m_RadialSegments; s++) {
                float theta = (2.0f * glm::pi<float>() * s) / m_RadialSegments;
                
                float px = xOffset + cos(theta) * currentRadius;
                float py = y;
                float pz = sin(theta) * currentRadius;

                glm::vec3 normal = glm::normalize(glm::vec3(cos(theta), 0.0f, sin(theta)));

                float u = (float)s / m_RadialSegments;
                float v = y / m_Height * m_Segments; 

                mesh->vertex_data.push_back(px);
                mesh->vertex_data.push_back(py);
                mesh->vertex_data.push_back(pz);
                mesh->vertex_data.push_back(normal.x);
                mesh->vertex_data.push_back(normal.y);
                mesh->vertex_data.push_back(normal.z);
                mesh->vertex_data.push_back(u);
                mesh->vertex_data.push_back(v);
            }
        }

        mesh->vertexCount = totalRings * (m_RadialSegments + 1);

        // Generate Indices
        for (int r = 0; r < totalRings - 1; r++) {
            for (int s = 0; s < m_RadialSegments; s++) {
                int current = r * (m_RadialSegments + 1) + s;
                int next = current + (m_RadialSegments + 1);

                mesh->indices.push_back(current);
                mesh->indices.push_back(next);
                mesh->indices.push_back(current + 1);

                mesh->indices.push_back(current + 1);
                mesh->indices.push_back(next);
                mesh->indices.push_back(next + 1);
            }
        }
        
        mesh->upload();
        
        modelRenderer->active = true;
        modelRenderer->meshes.push_back(std::move(mesh));
    }

    void OnUpdate(float deltaTime) override {
        currentTime += deltaTime;

        // Update dynamic uniforms
        if (Components::MaterialOverride* material = &registry->GetComponent<Components::MaterialOverride>(entityID)) {
            material->Uniforms["uTime"] = currentTime;
            
            // --- NEW ---
            // You MUST pass the Camera's position for specular lighting to work without going black.
            // If your engine allows you to get the active camera transform, use that:
            // vec3 camPos = activeCameraTransform.Position;
            // material->Uniforms["uViewPos"] = std::array<float, 3>{camPos.x, camPos.y, camPos.z};
            
            // For testing, supply a static fallback position so it doesn't crash to black:
            material->Uniforms["uViewPos"] = std::array<float, 3>{0.0f, 10.0f, 30.0f}; 
        }
    }
};

// --- The Factory Function ---
extern "C" __declspec(dllexport) Engine::Scripting::NativeScript* CreateScript() {
    return new BambooGenerator();
}
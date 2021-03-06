
#include "stdafx.hpp"

#include "Scene/ReflectionProbe.hpp"
#include "Scene/MeshPrefab.hpp"
#include "Scene/GameObject.hpp"

namespace flex
{
	ReflectionProbe::ReflectionProbe(bool visible) :
		m_Visible(visible)
	{
	}

	ReflectionProbe::~ReflectionProbe()
	{
	}

	void ReflectionProbe::Initialize(const GameContext& gameContext)
	{
		// Reflective chrome ball material
		Renderer::MaterialCreateInfo reflectionProbeMaterialCreateInfo = {};
		reflectionProbeMaterialCreateInfo.name = "Reflection probe ball";
		reflectionProbeMaterialCreateInfo.shaderName = "pbr";
		reflectionProbeMaterialCreateInfo.constAlbedo = glm::vec3(0.75f, 0.75f, 0.75f);
		reflectionProbeMaterialCreateInfo.constMetallic = 1.0f;
		reflectionProbeMaterialCreateInfo.constRoughness = 0.0f;
		reflectionProbeMaterialCreateInfo.constAO = 1.0f;
		MaterialID reflectionProbeMaterialID = gameContext.renderer->InitializeMaterial(gameContext, &reflectionProbeMaterialCreateInfo);

		// Probe capture material
		Renderer::MaterialCreateInfo probeCaptureMatCreateInfo = {};
		probeCaptureMatCreateInfo.name = "Reflection probe capture";
		probeCaptureMatCreateInfo.shaderName = "deferred_combine_cubemap";
		probeCaptureMatCreateInfo.generateReflectionProbeMaps = true;
		probeCaptureMatCreateInfo.generateHDRCubemapSampler = true;
		probeCaptureMatCreateInfo.generatedCubemapSize = glm::uvec2(512.0f, 512.0f); // TODO: Add support for non-512.0f size
		probeCaptureMatCreateInfo.generateCubemapDepthBuffers = true;
		probeCaptureMatCreateInfo.enableIrradianceSampler = true;
		probeCaptureMatCreateInfo.generateIrradianceSampler = true;
		probeCaptureMatCreateInfo.generatedIrradianceCubemapSize = { 32, 32 };
		probeCaptureMatCreateInfo.enablePrefilteredMap = true;
		probeCaptureMatCreateInfo.generatePrefilteredMap = true;
		probeCaptureMatCreateInfo.generatedPrefilteredCubemapSize = { 128, 128 };
		probeCaptureMatCreateInfo.enableBRDFLUT = true;
		probeCaptureMatCreateInfo.frameBuffers = {
			{ "positionMetallicFrameBufferSampler", nullptr },
			{ "normalRoughnessFrameBufferSampler", nullptr },
			{ "albedoAOFrameBufferSampler", nullptr },
		};
		MaterialID probeCaptureMaterialID = gameContext.renderer->InitializeMaterial(gameContext, &probeCaptureMatCreateInfo);



		m_SphereMesh = new MeshPrefab(reflectionProbeMaterialID, "Reflection probe");
		m_SphereMesh->LoadFromFile(gameContext, RESOURCE_LOCATION + "models/sphere.fbx", true, true);
		AddChild(m_SphereMesh);
		if (!m_Visible)
		{
			gameContext.renderer->SetRenderObjectVisible(m_SphereMesh->GetRenderID(), m_Visible);
		}

		m_Capture = new GameObject();
		Renderer::RenderObjectCreateInfo captuerObjectCreateInfo = {};
		captuerObjectCreateInfo.vertexBufferData = nullptr;
		captuerObjectCreateInfo.materialID = probeCaptureMaterialID;
		captuerObjectCreateInfo.name = "Reflection probe capture object";
		captuerObjectCreateInfo.transform = &m_Transform;
		
		RenderID captureRenderID = gameContext.renderer->InitializeRenderObject(gameContext, &captuerObjectCreateInfo);
		m_Capture->SetRenderID(captureRenderID);
		gameContext.renderer->SetRenderObjectVisible(captureRenderID, false);

		AddChild(m_Capture);
	}

	void ReflectionProbe::PostInitialize(const GameContext& gameContext)
	{
		// TODO: Do this automatically for all children !?!??!
		gameContext.renderer->PostInitializeRenderObject(gameContext, m_RenderID);
		gameContext.renderer->PostInitializeRenderObject(gameContext, m_SphereMesh->GetRenderID());
		gameContext.renderer->PostInitializeRenderObject(gameContext, m_Capture->GetRenderID());
	}

	void ReflectionProbe::Update(const GameContext& gameContext)
	{
		UNREFERENCED_PARAMETER(gameContext);
	}

	void ReflectionProbe::Destroy(const GameContext& gameContext)
	{
		UNREFERENCED_PARAMETER(gameContext);
	}

	void ReflectionProbe::SetSphereVisible(bool visible, const GameContext& gameContext)
	{
		m_Visible = visible;
		gameContext.renderer->SetRenderObjectVisible(m_SphereMesh->GetRenderID(), visible);
	}
}
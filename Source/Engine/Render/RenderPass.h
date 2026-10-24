#pragma once

#include <Engine/Core/Engine.h>
#include <Runtime/Definitions/Types.h>
#include <Runtime/Reflection/ReflectObject.h>
#include <Engine/Render/RenderFrame.h>

namespace Tourqe::Engine {
	class GraphicsSubsystem;

	enum class RenderStage : i32 {
		PreScene = 0,
		Scene = 1000,
		PostProcess = 2000,
		UI = 3000,
		Final = 4000
	};

	TCLASS();
	class RenderPass : public Runtime::Reflection::ReflectObject {
		GENERATE_CLASS(RenderPass)
	public:
		virtual ~RenderPass() = default;

		virtual void OnInitialize() = 0;
		virtual void OnDestroy() = 0;
		virtual void Execute(const RenderFrame& frame) = 0;

		virtual i32 GetOrder() const { return 0; }
		virtual RenderStage GetStage() const { return RenderStage::Scene; }

		void SetEngine(Engine* engine) { _engine = engine; }
	protected:
		Engine* GetEngine() const { return _engine; }
	private:
		Engine* _engine;
		
		friend class GraphicsSubsystem;
	};
}
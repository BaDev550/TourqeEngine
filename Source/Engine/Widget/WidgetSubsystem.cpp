#include "WidgetSubsystem.h"

#include <Engine/Core/Engine.h>
#include <Engine/Reflection/ReflectionSystem.h>
#include <Engine/Graphics/GraphicsSubsystem.h>
#include <Engine/Platform/PlatformSystem.h>
#include <Engine/Render/RenderFrame.h>

#include <Engine/World/WorldSubsystem.h>
#include <Runtime/Memory/Allocator.h>
#include <Runtime/Definitions/Profiler.h>

#include <imgui.h>

namespace Tourqe::Engine {
    Runtime::Result WidgetSubsystem::OnCreate(Engine* engine) {
		Subsystem::OnCreate(engine);

        auto* reflectionSystem = _engine->GetReflectionSystem();

        reflectionSystem->ForEachWithBase<Widget>([this](const Runtime::Reflection::Type& type) {
            Widget* widget = static_cast<Widget*>(type.Create());
			if (!widget) {
				Logger::Info("WidgetSubsystem", "failed to create '{}' widget", widget->GetClassName().data());
				return;
			}

            _widgetLookupTable[widget->GetClassName().data()] = _widgets.size();
            _widgets.push_back(std::move(widget));
			Logger::Info("WidgetSubsystem", "'{}' widget added", widget->GetClassName().data());
            });

        return Runtime::Result();
    }

    void Tourqe::Engine::WidgetSubsystem::OnUpdate() {
		auto system = _engine->GetSubsystem<GraphicsSubsystem>();
		
		system->Submit([this](Runtime::RHI::CommandBuffer* cmd) {
			{
				BEGIN_PROFILE_SCOPE("WidgetPass");

				auto renderSystem = _engine->GetSubsystem<GraphicsSubsystem>();
				auto platform = _engine->GetPlatformSystem()->GetPlatform();
				auto imgui = renderSystem->GetImGuiContext();

				RenderFrame frame{};
				frame.cmd = cmd;
				frame.color = renderSystem->GetSwapchain()->GetColorAttachments()[renderSystem->GetImageIndex()];
				frame.depth = renderSystem->GetSwapchain()->GetDepthAttachment();
				frame.extent = renderSystem->GetSwapchain()->GetExtent();
				frame.frameIndex = renderSystem->GetFrameIndex();
				frame.frameDataBufferIndex = 0;

				Runtime::RHI::ColorAttachment attachment{};
				attachment.image = frame.color;
				attachment.loadOp = Runtime::RHI::ImageLoadOp::Load;
				attachment.clearColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

				frame.cmd->TransitionImageLayout(frame.color, Runtime::RHI::ImageLayout::ColorAttachment);
				frame.cmd->TransitionImageLayout(frame.depth, Runtime::RHI::ImageLayout::DepthAttachment);

				Runtime::RHI::RenderingSubmitInfo info{};
				info.colorAttachments = { attachment };
				info.depthAttachment = frame.depth;
				info.extent = frame.extent;

				Runtime::RHI::PassDebugInfo debug{};
				debug.label = "WidgetPass";
				debug.color = glm::vec3(1.0f, 1.0f, 1.0f);

				platform->ImGuiNewFrame();
				imgui->Begin();
				ImGui::NewFrame();

				frame.cmd->BeginDynamicRendering(info, &debug);

				for (auto& widget : _widgets) {
					widget->Draw();
				}

				ImGui::Render();
				imgui->Flush(cmd);
				frame.cmd->EndDynamicRendering();
				frame.cmd->TransitionImageLayout(frame.color, Runtime::RHI::ImageLayout::PresentSrc);
			}
			});
    }

    void Tourqe::Engine::WidgetSubsystem::OnDestroy() {
		Subsystem::OnDestroy();
        for (auto& widget : _widgets) {
            Runtime::Mem::Allocator::Destroy(widget);
        }
    }

    void Tourqe::Engine::WidgetSubsystem::GetDependencyGraph(DependencyGraph& graph) {
        graph.Require<GraphicsSubsystem>(DependencyOrder::After);
		graph.Require<WorldSubsystem>(DependencyOrder::After);
    }
}
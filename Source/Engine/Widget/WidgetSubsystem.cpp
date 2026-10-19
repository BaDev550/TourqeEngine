#include "WidgetSubsystem.h"

#include <Engine/Core/Engine.h>
#include <Engine/Reflection/ReflectionSystem.h>
#include <Engine/Renderer/RenderSubsystem.h>
#include <Engine/World/WorldSubsystem.h>
#include <Runtime/Memory/Memory.h>

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
        auto* renderSystem = _engine->GetSubsystem<RenderSubsystem>();

        renderSystem->Submit([=](Runtime::RHI::CommandBuffer* cmd) {
			RenderGraph& graph = renderSystem->GetRenderGraph();

			u32 width = (u32)renderSystem->GetSwapchain()->GetExtent().x;
			u32 height = (u32)renderSystem->GetSwapchain()->GetExtent().y;

			auto backbuffer = graph.ImportImage(
				"BackBuffer",
				renderSystem->GetSwapchain()->GetColorAttachments()[renderSystem->GetImageIndex()],
				Runtime::RHI::ImageLayout::Undefined,
				Runtime::RHI::ImageLayout::PresentSrc
			);

			auto depthbuffer = graph.ImportImage(
				"DepthBuffer",
				renderSystem->GetSwapchain()->GetDepthAttachment(),
				Runtime::RHI::ImageLayout::Undefined,
				Runtime::RHI::ImageLayout::DepthAttachment
			);

			graph.AddPass("Widget",
				[&](RenderPassBuilder& builder) {
					builder.Write(backbuffer, Runtime::RHI::ImageLayout::ColorAttachment);
					builder.Write(depthbuffer, Runtime::RHI::ImageLayout::DepthAttachment);
					builder.ForceRender();
				},
				[=](Runtime::RHI::CommandBuffer* cmd, const RenderGraphResourceContainer& res) {
					auto* color = res.GetImage(backbuffer);
					auto* depth = res.GetImage(depthbuffer);

					Runtime::RHI::ColorAttachment colorAttachment{};
					colorAttachment.image = color;
					colorAttachment.loadOp = Runtime::RHI::ImageLoadOp::Load;
					colorAttachment.clearColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

					Runtime::RHI::RenderingSubmitInfo info{};
					info.colorAttachments = { colorAttachment };
					info.depthAttachment = depth;
					info.extent = glm::vec2(width, height);

					Runtime::RHI::PassDebugInfo debugInfo{};
					debugInfo.label = "WidgetPass";
					debugInfo.color = glm::vec3(1.0f, 0.0f, 0.0f);

					cmd->BeginDynamicRendering(info, &debugInfo);

					renderSystem->GetImGuiContext()->Begin();
					ImGui::NewFrame();
					for (auto& widget : _widgets) {
						widget->Draw();
					}
					ImGui::Render();
					renderSystem->GetImGuiContext()->Flush(cmd);

					cmd->EndDynamicRendering();
				});
            });
    }

    void Tourqe::Engine::WidgetSubsystem::OnDestroy() {
		Subsystem::OnDestroy();
        for (auto& widget : _widgets) {
            Runtime::Mem::Allocator::Destroy(widget);
        }
    }

    void Tourqe::Engine::WidgetSubsystem::GetDependencyGraph(DependencyGraph& graph) {
        graph.Require<RenderSubsystem>(DependencyOrder::After);
		graph.Require<WorldSubsystem>(DependencyOrder::After);
    }
}
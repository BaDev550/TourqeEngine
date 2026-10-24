#pragma once

#include <Engine/Render/RenderItem.h>
#include <array>

namespace Tourqe::Engine {
	enum class RenderQueue : u32 {
		Opaque,
		Transparent,
		Sprite,
		UI,
		Count
	};

	class RenderQueues {
	public:
		void Push(RenderQueue queue, const RenderItem& item) { _items[static_cast<u32>(queue)].push_back(item); }
		void Clear() { for (auto& queue : _items) queue.clear(); }
		const std::vector<RenderItem>& Get(RenderQueue queue) const { return _items[static_cast<u32>(queue)]; }
	private:
		std::array<std::vector<RenderItem>, static_cast<usize>(RenderQueue::Count)> _items;
	};
}
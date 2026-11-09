#include <Runtime/Memory/Allocator.h>
#include <Runtime/Definitions/Logger.h>

namespace Runtime::Mem {
	struct MemBlock {
		usize size;
		usize alignment;
	};

	usize AlignUp(usize value, usize align) { return (value + (align - 1)) & ~(align - 1); }

	void* Allocator::Allocate(usize size, usize align) {
		const usize eff = align < alignof(std::max_align_t) ? alignof(std::max_align_t) : align;
		const usize headerSize = AlignUp(sizeof(MemBlock), eff);

		void* base = ::operator new(headerSize + size, std::align_val_t{ eff }, std::nothrow);
		if (!base) {
			Logger::Fatal("Allocator", "Out of memory requesting {} bytes", size);
			return nullptr;
		}

		void* user = static_cast<c8*>(base) + headerSize;

		MemBlock* pHeader = reinterpret_cast<MemBlock*>(static_cast<c8*>(user) - sizeof(MemBlock));
		pHeader->size = size;
		pHeader->alignment = eff;

		return user;
	}

	void Allocator::Free(void* ptr) {
		if (!ptr)
			return;

		MemBlock* pHeader = reinterpret_cast<MemBlock*>(static_cast<c8*>(ptr) - sizeof(MemBlock));
		const usize eff = pHeader->alignment;
		const usize headerSize = AlignUp(sizeof(MemBlock), eff);
		void* base = static_cast<c8*>(ptr) - headerSize;

		::operator delete(base, std::align_val_t{ eff });
	}
}
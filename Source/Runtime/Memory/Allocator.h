#pragma once
#include <Runtime/Definitions/Types.h>

#include <memory>
#include <unordered_map>

namespace Runtime::Mem {
	class RUNTIME_API Allocator final {
	public:
		static void* Allocate(usize size, usize align = 16);
		static void Free(void* ptr);

		template<typename T, typename... Args>
		static T* Construct(Args&&... args) {
			void* mem = Allocate(sizeof(T), alignof(T));
			return new (mem) T(std::forward<Args>(args)...);
		}

		template<typename T>
		static void Destroy(T* ptr) {
			if (!ptr) return;
			void* base = ptr;

			if constexpr (std::is_polymorphic_v<T>)
				base = dynamic_cast<void*>(ptr);

			ptr->~T();
			Free(base);
			ptr = nullptr;
		}
	};
}
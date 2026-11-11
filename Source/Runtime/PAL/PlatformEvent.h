#pragma once

#include <Runtime/Definitions/Types.h>
#include <Runtime/PAL/KeyCodes.h>
#include <functional>

namespace Runtime::PAL {
	enum class PlatformEventType : u8 {
		None = 0,
		WindowResize,
		WindowClose,
		KeyPressed,
		KeyReleased,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
	};

	class PlatformEvent {
	public:
		PlatformEvent() = default;
		
		void SetValue(u32 value) { _value = value; }
		[[nodiscard]] const u32 GetValue() const { return _value; }
		[[nodiscard]] virtual const PlatformEventType GetType() const = 0;
	private:
		u32 _value;
	};

	class PlatformKeyPreseedEvent final : public PlatformEvent {
	public:
		PlatformKeyPreseedEvent(KeyCode key) : _key(key) { }

		[[nodiscard]] KeyCode GetKeyCode() const { return _key; }
		[[nodiscard]] virtual const PlatformEventType GetType() const final { return PlatformEventType::KeyPressed; }
	private:
		KeyCode _key;
	};


	class PlatformKeyReleasedEvent final : public PlatformEvent {
	public:
		PlatformKeyReleasedEvent(KeyCode key) : _key(key) {}

		[[nodiscard]] KeyCode GetKeyCode() const { return _key; }
		[[nodiscard]] virtual const PlatformEventType GetType() const final { return PlatformEventType::KeyReleased; }
	private:
		KeyCode _key;
	};

	using WindowEventCallback = std::function<void(PlatformEvent*)>;
}
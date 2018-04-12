#ifndef VS_UTILS_HPP
#define VS_UTILS_HPP

namespace vs {
	class utils {
	public:

		template<class I>
		/// <summary>
		/// Function to release directx interfaces
		/// </summary>
		/// <param name="interface_to_release">Interface to release</param>
		static void safe_release(I **interface_to_release) {
			if (*interface_to_release != nullptr) {
				(*interface_to_release)->Release();
				(*interface_to_release) = nullptr;
			}
		}
	};
}

#endif //VS_UTILS_HPP
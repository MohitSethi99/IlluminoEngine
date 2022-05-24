#include "Illumino/Core/Core.h"

#include <EASTL/string.h>
#include <optick.h>

namespace IlluminoEngine
{
	class StringUtils
	{
	public:
		inline static eastl::string GetExtension(const eastl::string&& filepath)
		{
			OPTICK_EVENT();

			auto lastDot = filepath.find_last_of(".");
			return filepath.substr(lastDot + 1, filepath.size() - lastDot);
		}

		inline static eastl::string GetName(const eastl::string&& filepath)
		{
			OPTICK_EVENT();

			auto lastSlash = filepath.find_last_of("/\\");
			lastSlash = lastSlash == eastl::string::npos ? 0 : lastSlash + 1;
			auto lastDot = filepath.rfind('.');
			auto count = lastDot == eastl::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
			return filepath.substr(lastSlash, count);
		}

		inline static eastl::string GetNameWithExtension(const eastl::string&& filepath)
		{
			OPTICK_EVENT();

			auto lastSlash = filepath.find_last_of("/\\");
			lastSlash = lastSlash == eastl::string::npos ? 0 : lastSlash + 1;
			return filepath.substr(lastSlash, filepath.size());
		}
	};
}

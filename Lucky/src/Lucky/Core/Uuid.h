#pragma once

#include <xhash>

namespace Lucky
{
	class Uuid
	{
	public:
		Uuid();
		Uuid(uint64_t uuid);
		Uuid(const Uuid&) = default;

		operator uint64_t() const { return m_Uuid; }

	private:
		uint64_t m_Uuid;
	};
}

namespace std
{
	template<>
	struct hash<Lucky::Uuid>
	{
		std::size_t operator()(const Lucky::Uuid& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
}

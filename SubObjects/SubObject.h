#pragma once
struct D3D12_STATE_SUBOBJECT;

namespace sub_objects
{
	class SubObject
	{
	public:
		virtual D3D12_STATE_SUBOBJECT Get() const = 0 ;
	protected:
		SubObject();
		virtual ~SubObject();

	};
}


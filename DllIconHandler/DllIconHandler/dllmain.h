// dllmain.h : Declaration of module class.

class CDllIconHandlerModule : public ATL::CAtlDllModuleT< CDllIconHandlerModule >
{
public :
	DECLARE_LIBID(LIBID_DllIconHandlerLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DLLICONHANDLER, "{f460f82a-b79c-45e0-b3b6-b2aa18d5717b}")
};

extern class CDllIconHandlerModule _AtlModule;

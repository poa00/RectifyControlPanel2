#include "pch.h"
#include "CControlPanelNavLinks.h"
#include "CControlPanelNavLink.h"

CControlPanelNavLinks::CControlPanelNavLinks()
{
	m_dpaList = NULL;
	m_refCount = 1;
}

void NavLinksDPA_DeleteCB(CControlPanelNavLink* p, void* pData)
{
	delete p;
}

CControlPanelNavLinks::~CControlPanelNavLinks()
{
	if (this->m_dpaList != NULL) {
		DPA_DestroyCallback(this->m_dpaList, (PFNDAENUMCALLBACKCONST)NavLinksDPA_DeleteCB, NULL);
		this->m_dpaList = (HDPA)0x0;
	}
}

IFACEMETHODIMP CControlPanelNavLinks::QueryInterface(REFIID riid, __out void** ppv)
{
	*ppv = NULL;
	if (riid == IID_IUnknown)
	{
		*ppv = (IUnknown*)this;
		return S_OK;
	}
	return E_NOINTERFACE;
}
IFACEMETHODIMP_(ULONG) CControlPanelNavLinks::AddRef()
{
	return InterlockedIncrement(&m_refCount);
}
IFACEMETHODIMP_(ULONG) CControlPanelNavLinks::Release()
{
	ULONGLONG ref = InterlockedDecrement(&m_refCount);
	if (ref == 0)
	{
		delete this;
	}
	return ref;
}
HRESULT CControlPanelNavLinks::AddLinkShellEx(LPCWSTR name)
{
	CControlPanelNavLink* link = NULL;
	HRESULT hr = CControlPanelNavLink::Create(CPNAV_Normal, &link);
	if (SUCCEEDED(hr))
	{
		link->SetName(name);
		
		SHSTOCKICONINFO icon = {};
		icon.cbSize = sizeof(SHSTOCKICONINFO);
		SHGetStockIconInfo(SIID_SHIELD, 0x101, &icon);

		link->m_Icon = icon.hIcon;
		link->m_ExecType = CPNAVTYPE_ShellExec;
		//link->IsBulletPoint = 0;
		SHStrDupW(L"Update Rectify11", &link->m_Name);
		SHStrDupW(L"c:\\windows\\notepad.exe", &link->m_args);
		return Add(link);
	}
	else
	{
		return hr;
	}
}
HRESULT CControlPanelNavLinks::AddLinkControlPanel()
{
	return S_OK;
}

HRESULT CControlPanelNavLinks::Add(CControlPanelNavLink* link)
{
	if (m_dpaList == NULL)
	{
		m_dpaList = DPA_Create(10);
		if (m_dpaList == NULL)
		{
			return E_OUTOFMEMORY;
		}
	}
	if (DPA_InsertPtr(m_dpaList, 0x7fffffff, link) != -1)
	{
		return S_OK;
	}
	else
	{
		return E_OUTOFMEMORY;
	}
}
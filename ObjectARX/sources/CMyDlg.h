// ReSharper disable CppInconsistentNaming
#pragma once

#include "StdAfx.h"

class CMyDlg final : public CDialog  // NOLINT(cppcoreguidelines-special-member-functions)
{
	DECLARE_DYNAMIC(CMyDlg)

public:
	explicit CMyDlg(CWnd* pParent = nullptr);   // standard constructor
	CMyDlg(CMyDlg&) = delete;
	~CMyDlg() override;

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void OnBnClickedOk();
	BOOL OnInitDialog() override;
	void PostNcDestroy() override;
};

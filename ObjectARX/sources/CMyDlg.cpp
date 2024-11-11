// ReSharper disable CppInconsistentNaming
#include "stdafx.h"
#include "CMyDlg.h"
#include "common.h"
#include "Resource.h"

IMPLEMENT_DYNAMIC(CMyDlg, CDialog)

CMyDlg::CMyDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG1, pParent)
{

}

CMyDlg::~CMyDlg()
= default;

void CMyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMyDlg, CDialog)
	ON_BN_CLICKED(ID_OK, &CMyDlg::OnBnClickedOk)
END_MESSAGE_MAP()


BOOL CMyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}


void CMyDlg::PostNcDestroy()
{
	delete this;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void CMyDlg::OnBnClickedOk()
{
	ads_point ptSta;
	acedInitGet(RSG_NONULL, nullptr);
	auto res = acedGetPoint(nullptr, _T("Set start point: "), ptSta);
	if (res != RTNORM) return;
	acutPrintf(_T("OK\r\n"));

	ads_point ptEnd;
	acedInitGet(RSG_NONULL, nullptr);
	res = acedGetPoint(ptSta, _T("Set end point: "), ptEnd);
	if (res != RTNORM) return;
	acutPrintf(_T("OK\r\n"));

	const AcGePoint3d pt3Sta(ptSta[X], ptSta[Y], ptSta[Z]);
	const AcGePoint3d pt3End(ptEnd[X], ptEnd[Y], ptEnd[Z]);

	AcDbLine* pLine = nullptr;
	AcDbBlockTable* pTbl = nullptr;
	AcDbBlockTableRecord* pRec = nullptr;
	acDocManager->lockDocument(acDocManager->curDocument());
	{
		const auto pDb = acdbHostApplicationServices()->workingDatabase();
		pLine = new AcDbLine(pt3Sta, pt3End);

		
		auto stat = pDb->getBlockTable(pTbl, kForRead);
		if (stat != eOk) goto finally;

		stat = pTbl->getAt(MODEL_SPACE, pRec, kForWrite);
		if (stat != eOk) goto finally;

		pRec->appendAcDbEntity(pLine);
	}
	finally:
	if (pRec != nullptr) pRec->close();
	if (pTbl != nullptr) pTbl->close();
	if (pLine != nullptr) pLine->close();

	acDocManager->unlockDocument(acDocManager->curDocument());
	acutPrintf(_T("\r\n"));
}

/*************************************************************************\

Copyright 2018 Zhejiang University.
All Rights Reserved.

Permission to use, copy, modify and distribute this software and its
documentation for educational, research and non-profit purposes, without
fee, and without a written agreement is hereby granted, provided that the
above copyright notice and the following three paragraphs appear in all
copies.

IN NO EVENT SHALL ZHEJIANG UNIVERSITY BE
LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE
USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF ZHEJIANG
UNIVERSITY HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

ZHEJIANG UNIVERSITY SPECIFICALLY DISCLAIM ANY
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE
PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND ZHEJIANG UNIVERSITY
HAS NO OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT,
UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

The authors may be contacted via:

Mail:             College of Computer Science and Technology
Zhejiang Univeristy
Hangzhou, Zhejiang, China 310027

EMail:            tang_m@zju.edu.cn

\**************************************************************************/

#pragma once
#include "afxwin.h"


// CDlgWizzardCreate dialog

class CDlgWizzardCreate : public CDialog
{
	DECLARE_DYNAMIC(CDlgWizzardCreate)

public:
	CDlgWizzardCreate(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgWizzardCreate();

// Dialog Data
	enum { IDD = IDD_DLG_WIZZARD_CREATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPrev();
	CEdit m_editPath1;
	CEdit m_editPath2;
	afx_msg void OnBnClickedButtonOpenFile1();
	afx_msg void OnBnClickedButtonOpenFile2();
	afx_msg void OnBnClickedButtonNext();
};

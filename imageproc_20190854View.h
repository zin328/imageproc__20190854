﻿
// imageproc_20190854View.h: Cimageproc20190854View 클래스의 인터페이스
//

#pragma once


class Cimageproc20190854View : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	Cimageproc20190854View() noexcept;
	DECLARE_DYNCREATE(Cimageproc20190854View)

// 특성입니다.
public:
	Cimageproc20190854Doc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~Cimageproc20190854View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMenutest();
	afx_msg void OnPixeladd();
	afx_msg void OnPixelsub();
	afx_msg void OnPixeldiv();
	afx_msg void OnPixelmul();
	afx_msg void OnPixelHistoeq();
	afx_msg void OnPixelstr();
	afx_msg void OnPixelbinarization();
	afx_msg void OnPixeltwoimageadd();
	void LoadTwoImage();
	afx_msg void OnPixelimagesub();
	afx_msg void OnRegionSharpening();
	void convolve(unsigned char** inimg, unsigned char** outimg, int cols, int rows, float mask[][3], int bias, int depth);
	afx_msg void OnRegionSmoothing();
	afx_msg void OnRegionEmbossing();
	afx_msg void OnRegionPrewit();
	afx_msg void OnRegionRoberts();
	afx_msg void OnRegionSobel();
	afx_msg void OnRegionEverageFlitering();
	afx_msg void OnRegionMedianfiltering();
	
	
	afx_msg void OnGeometryZoominPixelCopy();
	afx_msg void OnMopologycolortogray();
	afx_msg void OnMopologybinarization();
	afx_msg void OnMopologyerosion();
	afx_msg void OnMopologydilation();
	afx_msg void OnMopologyopenning();
	afx_msg void OnMopologyclosing();
	void CopyResultToInput();
	afx_msg void OnGeometryZoominBinuerinterpolation();
	afx_msg void OnGeometryZoomoutSubsampling();
	afx_msg void OnGeometryZoomoutMeanSub();
	afx_msg void OnGeometryZoomoutAvg();
	afx_msg void OnGeometryRotation();
	afx_msg void OnGeometryMirror();
	afx_msg void OnGeometryFlip();
	afx_msg void OnGeometryWarrping();
	afx_msg void OnGeometryWarping();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnAviView();
	bool bAviMode;
	CString AviFileName;
	void LoadAviFile(CDC* PDC);
};

#ifndef _DEBUG  // imageproc_20190854View.cpp의 디버그 버전
inline Cimageproc20190854Doc* Cimageproc20190854View::GetDocument() const
   { return reinterpret_cast<Cimageproc20190854Doc*>(m_pDocument); }
#endif


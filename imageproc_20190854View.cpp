﻿
// imageproc_20190854View.cpp: Cimageproc20190854View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "imageproc_20190854.h"
#endif
#include<vfw.h>
#include "imageproc_20190854Doc.h"
#include "imageproc_20190854View.h"
#include "CAngleinputDialog.h"

#define MORPHING 8 
int viewMode;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cimageproc20190854View

IMPLEMENT_DYNCREATE(Cimageproc20190854View, CScrollView)

BEGIN_MESSAGE_MAP(Cimageproc20190854View, CScrollView)
	ON_COMMAND(ID_MENUTEST, &Cimageproc20190854View::OnMenutest)
	ON_COMMAND(ID_PIXELADD, &Cimageproc20190854View::OnPixeladd)
	ON_COMMAND(ID_PIXELSUB, &Cimageproc20190854View::OnPixelsub)
	ON_COMMAND(ID_PIXELDIV, &Cimageproc20190854View::OnPixeldiv)
	ON_COMMAND(ID_PIXELMUL, &Cimageproc20190854View::OnPixelmul)
	ON_COMMAND(ID_PIXEL_HISTOEQ, &Cimageproc20190854View::OnPixelHistoeq)
	ON_COMMAND(ID_PIXELSTR, &Cimageproc20190854View::OnPixelstr)
	ON_COMMAND(ID_PIXELBINARIZATION, &Cimageproc20190854View::OnPixelbinarization)
	ON_COMMAND(ID_PIXELTWOIMAGEADD, &Cimageproc20190854View::OnPixeltwoimageadd)
	ON_COMMAND(ID_PIXELIMAGESUB, &Cimageproc20190854View::OnPixelimagesub)
	ON_COMMAND(ID_REGION_SHARPENING, &Cimageproc20190854View::OnRegionSharpening)
	ON_COMMAND(ID_REGION_SMOOTHING, &Cimageproc20190854View::OnRegionSmoothing)
	ON_COMMAND(ID_REGION_EMBOSSING, &Cimageproc20190854View::OnRegionEmbossing)
	ON_COMMAND(ID_Region_Prewit, &Cimageproc20190854View::OnRegionPrewit)
	ON_COMMAND(ID_Region_Roberts, &Cimageproc20190854View::OnRegionRoberts)
	ON_COMMAND(ID_Region_Sobel, &Cimageproc20190854View::OnRegionSobel)
	
ON_COMMAND(ID_REGION_EVERAGE_FLITERING, &Cimageproc20190854View::OnRegionEverageFlitering)
ON_COMMAND(ID_REGION_MEDIANFILTERING, &Cimageproc20190854View::OnRegionMedianfiltering)
ON_COMMAND(ID_GEOMETRY_ZOOMIN_PIXEL_COPY, &Cimageproc20190854View::OnGeometryZoominPixelCopy)
ON_COMMAND(ID_MOPOLOGYCOLORTOGRAY, &Cimageproc20190854View::OnMopologycolortogray)
ON_COMMAND(ID_MOPOLOGYBINARIZATION, &Cimageproc20190854View::OnMopologybinarization)
ON_COMMAND(ID_MOPOLOGYEROSION, &Cimageproc20190854View::OnMopologyerosion)
ON_COMMAND(ID_MOPOLOGYDILATION, &Cimageproc20190854View::OnMopologydilation)
ON_COMMAND(ID_MOPOLOGYOPENNING, &Cimageproc20190854View::OnMopologyopenning)
ON_COMMAND(ID_MOPOLOGYCLOSING, &Cimageproc20190854View::OnMopologyclosing)
ON_COMMAND(ID_GEOMETRY_ZOOMIN_BINUERINTERPOLATION, &Cimageproc20190854View::OnGeometryZoominBinuerinterpolation)
ON_COMMAND(ID_GEOMETRY_ZOOMOUT_SUBSAMPLING, &Cimageproc20190854View::OnGeometryZoomoutSubsampling)
ON_COMMAND(ID_GEOMETRY_ZOOMOUT_MEAN_SUB, &Cimageproc20190854View::OnGeometryZoomoutMeanSub)
ON_COMMAND(ID_GEOMETRY_ZOOMOUT_AVG, &Cimageproc20190854View::OnGeometryZoomoutAvg)
ON_COMMAND(ID_GEOMETRY_ROTATION, &Cimageproc20190854View::OnGeometryRotation)
ON_COMMAND(ID_GEOMETRY_MIRROR, &Cimageproc20190854View::OnGeometryMirror)
ON_COMMAND(ID_GEOMETRY_FLIP, &Cimageproc20190854View::OnGeometryFlip)
//ON_COMMAND(ID_GEOMETRY_WARRPING, &Cimageproc20190854View::OnGeometryWarrping)
ON_COMMAND(ID_GEOMETRY_WARPING, &Cimageproc20190854View::OnGeometryWarping)
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_COMMAND(ID_AVI_VIEW, &Cimageproc20190854View::OnAviView)
ON_COMMAND(ID_GEOMETRY_MORPHING, &Cimageproc20190854View::OnGeometryMorphing)
END_MESSAGE_MAP()

// Cimageproc20190854View 생성/소멸

Cimageproc20190854View::Cimageproc20190854View() noexcept
{
	bAviMode = false;

}

Cimageproc20190854View::~Cimageproc20190854View()
{
}

BOOL Cimageproc20190854View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// Cimageproc20190854View 그리기

void Cimageproc20190854View::OnDraw(CDC* pDC)
{
	Cimageproc20190854Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	int x, y;

	if (bAviMode) 
	{
		//avi 파일 재생 
		LoadAviFile(pDC);
		bAviMode = false; 
		return;
	}

	if (pDoc->inputimg != NULL)
	{
		if (pDoc->depth == 1) {
			for (int y = 0; y < pDoc->ImageHeight; y++)
				for (int x = 0; x < pDoc->ImageWidth; x++)
					pDC->SetPixel(x, y, RGB(pDoc->inputimg[y][x], pDoc->inputimg[y][x], pDoc->inputimg[y][x]));
		}
		else {
			for (int y = 0; y < pDoc->ImageHeight; y++)
				for (int x = 0; x < pDoc->ImageWidth; x++)
					pDC->SetPixel(x, y, RGB(pDoc->inputimg[y][3 * x + 0], pDoc->inputimg[y][3*x+1], pDoc->inputimg[y][3 * x + 2]));
		}
	}
	if (pDoc->resultimg != NULL) {
		if (pDoc->depth == 1) {
			for (int y = 0; y < pDoc->ImageHeight; y++)
				for (int x = 0; x < pDoc->ImageWidth; x++)
					pDC->SetPixel(pDoc->ImageWidth + 20 + x, y, RGB(pDoc->resultimg[y][x], pDoc->resultimg[y][x], pDoc->resultimg[y][x]));
		}
		else
		{
			for (int y = 0; y < pDoc->ImageHeight; y++)
				for (int x = 0; x < pDoc->ImageWidth; x++)
					pDC->SetPixel(pDoc->ImageWidth + 20 + x, y, RGB(pDoc->resultimg[y][3 * x + 0], pDoc->resultimg[y][3 * x + 1], pDoc->resultimg[y][3 * x + 2]));
		}
	}
	if (pDoc->inputimg2 != NULL) {
		if (pDoc->depth == 1) {
			for (int y = 0; y < pDoc->ImageHeight; y++)
				for (int x = 0; x < pDoc->ImageWidth; x++)
					pDC->SetPixel(2 * (pDoc->ImageWidth + 20) + x, y, RGB(pDoc->inputimg2[y][x], pDoc->inputimg2[y][x], pDoc->inputimg2[y][x]));
		}
		else {
			for (int y = 0; y < pDoc->ImageHeight; y++)
				for (int x = 0; x < pDoc->ImageWidth; x++)
					pDC->SetPixel(2 * (pDoc->ImageWidth + 20) + x, y, RGB(pDoc->inputimg2[y][3 * x + 0], pDoc->inputimg2[y][3 * x + 1], pDoc->inputimg2[y][3 * x + 2]));
		}
	}
	if (pDoc->gresultimg != NULL) {
		if (pDoc->depth == 1) {
			for (int y = 0; y < pDoc->gImageHeight; y++)
				for (int x = 0; x < pDoc->gImageWidth; x++)
					pDC->SetPixel( x, pDoc->ImageHeight+20+y, RGB(pDoc->gresultimg[y][x], pDoc->gresultimg[y][x], pDoc->gresultimg[y][x]));
		}
		else {
			for (int y = 0; y < pDoc->gImageHeight; y++)
				for (int x = 0; x < pDoc->gImageWidth; x++)
					pDC->SetPixel( x, pDoc->ImageHeight + 20+y, RGB(pDoc->gresultimg[y][3 * x + 0], pDoc->gresultimg[y][3 * x + 1], pDoc->gresultimg[y][3 * x + 2]));
		}
	}
	if (viewMode == MORPHING) {
		for (int y = 0; y < pDoc->ImageHeight; y++)       // 두번째 입력 영상 출력 
			for (int x = 0; x < pDoc->ImageWidth; x++)
				pDC->SetPixel(x + pDoc->ImageWidth + 30, y, 
					RGB(pDoc->inputimg2[y][x],
						pDoc->inputimg2[y][x],
						pDoc->inputimg2[y][x]));
		for (int i = 0; i < 10; i++)
			for (int y = 0; y < pDoc->ImageHeight; y++)       // 모핑 결과 출력 
				for (int x = 0; x < pDoc->ImageWidth; x++)
					pDC->SetPixel(x + pDoc->ImageWidth * 2 + 60, y,
						RGB(pDoc->morhedImg[i][y][x],
							pDoc->morhedImg[i][y][x],
							pDoc->morhedImg[i][y][x]));
	}
}
   

void Cimageproc20190854View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy =2048;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// Cimageproc20190854View 진단

#ifdef _DEBUG
void Cimageproc20190854View::AssertValid() const
{
	CScrollView::AssertValid();
}

void Cimageproc20190854View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

Cimageproc20190854Doc* Cimageproc20190854View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cimageproc20190854Doc)));
	return (Cimageproc20190854Doc*)m_pDocument;
}
#endif //_DEBUG


// Cimageproc20190854View 메시지 처리기


void Cimageproc20190854View::OnMenutest()
{
	AfxMessageBox("안녕하세요 메뉴출력입니다");
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void Cimageproc20190854View::OnPixeladd()
{
	Cimageproc20190854Doc* pDoc = GetDocument();
	if (pDoc->inputimg == NULL)return;

	int x, y,value;

	for(y=0;y<pDoc->ImageHeight;y++)
		for (x = 0; x < pDoc->ImageWidth; x++)
			{
			if (pDoc->depth == 1) {
				value = pDoc->inputimg[y][x] + 100;
				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][x] = value;
			}
			else {
				value = pDoc->inputimg[y][3*x+0] + 100;
				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][3*x+0] = value;

				value = pDoc->inputimg[y][3 * x + 1] + 100;
				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][3 * x + 1] = value;

				value = pDoc->inputimg[y][3 * x + 2] + 100;
				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][3 * x + 2] = value;
			}
		}
	Invalidate();

	
}


void Cimageproc20190854View::OnPixelsub()
{
	Cimageproc20190854Doc* pDoc = GetDocument();
	int x, y;
	int value;
	for (y = 0; y < pDoc->ImageHeight; y++) {
		for (x = 0; x < pDoc->ImageWidth; x++) {
			if (pDoc->depth == 1) {	 //흑백 이미지
				value = pDoc->inputimg[y][x] - 100;
				if (value >= 255) value = 255;
				else if (value < 0) value = 0;
				pDoc->resultimg[y][x] = value;
			}
			else {	 //컬러 이미지
				value = pDoc->inputimg[y][3 * x + 0] - 100;
				if (value >= 255) value = 255;
				else if (value < 0) value = 0;
				pDoc->resultimg[y][3 * x + 0] = value;

				value = pDoc->inputimg[y][3 * x + 1] - 100;
				if (value >= 255) value = 255;
				else if (value < 0) value = 0;
				pDoc->resultimg[y][3 * x + 1] = value;

				value = pDoc->inputimg[y][3 * x + 2] - 100;
				if (value >= 255) value = 255;
				else if (value < 0) value = 0;
				pDoc->resultimg[y][3 * x + 2] = value;
			}
		}
	}
	Invalidate();
}


void Cimageproc20190854View::OnPixeldiv()
{

	Cimageproc20190854Doc* pDoc = GetDocument(); 
	int x, y;
	int value;
	for (y = 0; y < pDoc->ImageHeight; y++) {
		for (x = 0; x < pDoc->ImageWidth; x++) {
			if (pDoc->depth == 1) {	 //흑백 이미지
				value = pDoc->inputimg[y][x] / 1.5;
				if (value >= 255) value = 255;
				else if (value < 0) value = 0;
				pDoc->resultimg[y][x] = value;
			}
			else {	 //컬러 이미지
				value = pDoc->inputimg[y][3 * x + 0] / 1.5;
				if (value >= 255) value = 255;
				else if (value < 0) value = 0;
				pDoc->resultimg[y][3 * x + 0] = value; 

				value = pDoc->inputimg[y][3 * x + 1] / 1.5;
				if (value >= 255) value = 255;
				else if (value < 0) value = 0;
				pDoc->resultimg[y][3 * x + 1] = value; 

				value = pDoc->inputimg[y][3 * x + 2] / 1.5;
				if (value >= 255) value = 255;
				else if (value < 0) value = 0;
				pDoc->resultimg[y][3 * x + 2] = value; 
			}
		}
	}
	Invalidate();
}



void Cimageproc20190854View::OnPixelmul()
{
	
	Cimageproc20190854Doc* pDoc = GetDocument();
	int x, y;
	int value;
	for (y = 0; y < pDoc->ImageHeight; y++) {
		for (x = 0; x < pDoc->ImageWidth; x++) {
			if (pDoc->depth == 1) {	 //흑백 이미지
				value = pDoc->inputimg[y][x] * 1.5;
				if (value >= 255) value = 255;
				else if (value < 0) value = 0;
				pDoc->resultimg[y][x] = value;
			}
			else {	 //컬러 이미지
				value = pDoc->inputimg[y][3 * x + 0] * 1.5;
				if (value >= 255) value = 255;
				else if (value < 0) value = 0;
				pDoc->resultimg[y][3 * x + 0] = value;

				value = pDoc->inputimg[y][3 * x + 1] * 1.5;
				if (value >= 255) value = 255;
				else if (value < 0) value = 0;
				pDoc->resultimg[y][3 * x + 1] = value;

				value = pDoc->inputimg[y][3 * x + 2] * 1.5;
				if (value >= 255) value = 255;
				else if (value < 0) value = 0;
				pDoc->resultimg[y][3 * x + 2] = value;
			}
		}
	}
	Invalidate();
}





void Cimageproc20190854View::OnPixelHistoeq()
{
	Cimageproc20190854Doc* pDoc = GetDocument();
	int x, y, k;
	int hist[256]{}, sum[256]{};
	int rhist[256]{}, rsum[256]{}, ghist[256]{}, gsum[256]{}, bhist[256]{}, bsum[256]{};
	int acc_hist = 0;
	int r_acc_hist = 0, g_acc_hist = 0, b_acc_hist = 0;
	float N = (float)(pDoc->ImageHeight * pDoc->ImageWidth);	//영상의 전체 픽셀 수

	//명암값 빈도수 조사
	for (y = 0; y < pDoc->ImageHeight; y++) {
		for (x = 0; x < pDoc->ImageWidth; x++) {
			if (pDoc->depth == 1) {	 //흑백 이미지
				hist[pDoc->inputimg[y][x]]++;
			}
			else {	 //컬러 이미지
				rhist[pDoc->inputimg[y][3 * x + 0]]++;
				ghist[pDoc->inputimg[y][3 * x + 1]]++;
				bhist[pDoc->inputimg[y][3 * x + 2]]++;
			}
		}
	}

	//누적된 히스토그램 합 계산
	for (k = 0; k < 256; k++) {
		if (pDoc->depth == 1) {	 //흑백 이미지
			acc_hist += hist[k];
			sum[k] = acc_hist;
		}
		else {	 //컬러 이미지
			r_acc_hist += rhist[k];
			rsum[k] = r_acc_hist;

			g_acc_hist += ghist[k];
			gsum[k] = g_acc_hist;

			b_acc_hist += bhist[k];
			bsum[k] = b_acc_hist;
		}
	}

	for (y = 0; y < pDoc->ImageHeight; y++) {
		for (x = 0; x < pDoc->ImageWidth; x++) {
			if (pDoc->depth == 1) {	 //흑백 이미지
				k = pDoc->inputimg[y][x];
				pDoc->resultimg[y][x] = (unsigned char)(sum[k] / N * 255);
			}
			else {	 //컬러 이미지
				k = pDoc->inputimg[y][3 * x + 0];
				pDoc->resultimg[y][3 * x + 0] = (unsigned char)(rsum[k] / N * 255);

				k = pDoc->inputimg[y][3 * x + 1];
				pDoc->resultimg[y][3 * x + 1] = (unsigned char)(gsum[k] / N * 255);

				k = pDoc->inputimg[y][3 * x + 2];
				pDoc->resultimg[y][3 * x + 2] = (unsigned char)(bsum[k] / N * 255);
			}
		}
	}

	Invalidate();
}

void Cimageproc20190854View::OnPixelstr()
{
Cimageproc20190854Doc* pDoc = GetDocument();
//변수 선언
int min = 256, max = -1;
int rmin = 256, rmax = -1, gmin = 256, gmax = -1, bmin = 256, bmax = -1;
int x, y;
int p;

//최대, 최소 값 검색
for (y = 0; y < pDoc->ImageHeight; y++) {
	for (x = 0; x < pDoc->ImageWidth; x++) {
		if (pDoc->depth == 1) {	 //흑백 이미지
			if (pDoc->inputimg[y][x] < min) min = pDoc->inputimg[y][x];
			if (pDoc->inputimg[y][x] > max) max = pDoc->inputimg[y][x];
		}
		else {	 //컬러 이미지
			if (pDoc->inputimg[y][3 * x + 0] < rmin) rmin = pDoc->inputimg[y][3 * x + 0];
			if (pDoc->inputimg[y][3 * x + 0] > rmax) rmax = pDoc->inputimg[y][3 * x + 0];

			if (pDoc->inputimg[y][3 * x + 1] < gmin) gmin = pDoc->inputimg[y][3 * x + 1];
			if (pDoc->inputimg[y][3 * x + 1] > gmax) gmax = pDoc->inputimg[y][3 * x + 1];

			if (pDoc->inputimg[y][3 * x + 2] < bmin) bmin = pDoc->inputimg[y][3 * x + 2];
			if (pDoc->inputimg[y][3 * x + 2] > bmax) bmax = pDoc->inputimg[y][3 * x + 2];
		}
	}
}

//P' = (float)(P-min)/(max-min)*255
for (y = 0; y < pDoc->ImageHeight; y++) {
	for (x = 0; x < pDoc->ImageWidth; x++) {
		if (pDoc->depth == 1) {	 //흑백 이미지
			p = pDoc->inputimg[y][x];
			pDoc->resultimg[y][x] = (float)(p - min) / (max - min) * 255;
		}
		else {	 //컬러 이미지
			p = pDoc->inputimg[y][3 * x + 0];
			pDoc->resultimg[y][3 * x + 0] = (float)(p - rmin) / (rmax - rmin) * 255;

			p = pDoc->inputimg[y][3 * x + 1];
			pDoc->resultimg[y][3 * x + 1] = (float)(p - gmin) / (gmax - gmin) * 255;

			p = pDoc->inputimg[y][x * 3 + 2];
			pDoc->resultimg[y][3 * x + 2] = (float)(p - bmin) / (bmax - bmin) * 255;
		}
	}
}

Invalidate();
}


void Cimageproc20190854View::OnPixelbinarization()
{
	Cimageproc20190854Doc* pDoc = GetDocument();

	int i, x, y;
	int threshold = 100;
	for (y = 0; y < pDoc->ImageHeight; y++)
		for (x = 0; x <pDoc->ImageWidth; x++)
		{
			if (pDoc->inputimg[y][x] > threshold) pDoc->resultimg[y][x] = 255;
			else                                                    pDoc->resultimg[y][x] = 0;
		}
	 Invalidate(); 
}


void Cimageproc20190854View::OnPixeltwoimageadd()
{
	Cimageproc20190854Doc* pDoc = GetDocument();
 
	
	
	CFileDialog dlg(true);
	CFile file;

	if (dlg.DoModal() == IDOK) {

		file.Open(dlg.GetPathName(), CFile::modeRead);
		CArchive ar(&file, CArchive::load);
		pDoc->LoadSecondImageFile(ar);
		file.Close();

		int x, y;
		for (y = 0; y < pDoc->ImageHeight; y++) {
			for (x = 0; x < pDoc->ImageWidth; x++) {
				if (pDoc->depth == 1) {	 //흑백 이미지
					pDoc->resultimg[y][x] = 0.5 * pDoc->inputimg[y][x] + 0.5 * pDoc->inputimg2[y][x];
				}
				else {	 //컬러 이미지
					pDoc->resultimg[y][x * 3 + 0] = 0.5 * pDoc->inputimg[y][x * 3 + 0] + 0.5 * pDoc->inputimg2[y][x * 3 + 0];
					pDoc->resultimg[y][x * 3 + 1] = 0.5 * pDoc->inputimg[y][x * 3 + 1] + 0.5 * pDoc->inputimg2[y][x * 3 + 1];
					pDoc->resultimg[y][x * 3 + 2] = 0.5 * pDoc->inputimg[y][x * 3 + 2] + 0.5 * pDoc->inputimg2[y][x * 3 + 2];
				}
			}
		}

		Invalidate();
	}
}


void Cimageproc20190854View::LoadTwoImage()
{
	Cimageproc20190854Doc* pDoc = GetDocument();
	CFileDialog dlg(TRUE);

	if (dlg.DoModal() == IDOK) {
		CFile file;
		file.Open(dlg.GetPathName(), CFile::modeRead); //  파일 열기
		CArchive ar(&file, CArchive::load);
		pDoc->LoadSecondImageFile(ar);

		file.Close();
	}
	
}


void Cimageproc20190854View::OnPixelimagesub()
{
	Cimageproc20190854Doc* pDoc = GetDocument();
	CFileDialog dlg(true);
	CFile file;

	if (dlg.DoModal() == IDOK) {

		file.Open(dlg.GetPathName(), CFile::modeRead);
		CArchive ar(&file, CArchive::load);
		pDoc->LoadSecondImageFile(ar);
		file.Close();

		int x, y;
		for (y = 0; y < pDoc->ImageHeight; y++) {
			for (x = 0; x < pDoc->ImageWidth; x++) {
				if (pDoc->depth == 1) {	 //흑백 이미지
					pDoc->resultimg[y][x] = abs(pDoc->inputimg[y][x] - pDoc->inputimg2[y][x]);
				}
				else {	 //컬러 이미지
					pDoc->resultimg[y][x * 3 + 0] = abs(pDoc->inputimg[y][x * 3 + 0] - pDoc->inputimg2[y][x * 3 + 0]);
					pDoc->resultimg[y][x * 3 + 1] = abs(pDoc->inputimg[y][x * 3 + 1] - pDoc->inputimg2[y][x * 3 + 1]);
					pDoc->resultimg[y][x * 3 + 2] = abs(pDoc->inputimg[y][x * 3 + 2] - pDoc->inputimg2[y][x * 3 + 2]);
				}
			}
		}

		Invalidate();
	}
}

void Cimageproc20190854View::convolve(unsigned char** inimg, unsigned char** outimg, int cols, int rows, float mask[][3], int bias, int depth)
{
	int x, y;
	int i, j;
	int sum,rsum,gsum,bsum;

	for (y = 1; y < rows-1; y++)
		for (x = 1; x < cols-1; x++) 
		{
			if (depth == 1) {


				sum = 0;
				for (j = 0; j < 3; j++)
					for (i = 0; i < 3; i++)
					{
						sum += (inimg[y + j - 1][x + i - 1] * mask[j][i]);
					}
				sum += bias;
				if (sum > 255)sum = 255;
				else if (sum < 0)sum = 0;

				outimg[y][x] = sum;
			}
			else {
				rsum = 0; gsum = 0; bsum = 0;
				for (j = 0; j < 3; j++)
					for (i = 0; i < 3; i++)
					{
						rsum += (inimg[y + j - 1][3*(x + i - 1)+0] * mask[j][i]);
						gsum += (inimg[y + j - 1][3 * (x + i - 1) + 1] * mask[j][i]);
						bsum += (inimg[y + j - 1][3 * (x + i - 1) + 2] * mask[j][i]);

					}
				rsum += bias;
				gsum += bias;
				bsum += bias;

				if (rsum > 255)rsum = 255;
				else if (rsum < 0)rsum = 0;
				if (gsum > 255)gsum = 255;
				else if (gsum < 0)gsum = 0;
				if (bsum > 255)bsum = 255;
				else if (bsum < 0)bsum = 0;


				outimg[y][3 * x + 0] = rsum;
				outimg[y][3 * x + 1] = gsum;
				outimg[y][3 * x + 2] = bsum;
			}

			

		}
}

void Cimageproc20190854View::OnRegionSharpening()
{
	Cimageproc20190854Doc* pDoc = GetDocument();
	float kernel[3][3] = { {0,-1,0},
						  {-1,5,-1},
						  {0,-1,0} };

	convolve(pDoc->inputimg, pDoc->resultimg, pDoc->ImageWidth, pDoc->ImageHeight, kernel, 0, pDoc->depth);
	
	Invalidate();
}

void Cimageproc20190854View::OnRegionSmoothing()
{
	Cimageproc20190854Doc* pDoc = GetDocument();
	float kernel[3][3] = {
		{1 / 9.0f, 1 / 9.0f, 1 / 9.0f},
		{1 / 9.0f, 1 / 9.0f, 1 / 9.0f},
		{1 / 9.0f, 1 / 9.0f, 1 / 9.0f}
	};

	convolve(pDoc->inputimg, pDoc->resultimg, pDoc->ImageWidth, pDoc->ImageHeight, kernel, 0, pDoc->depth);

	Invalidate();
}



void Cimageproc20190854View::OnRegionEmbossing()
{
	Cimageproc20190854Doc* pDoc = GetDocument();
	float kernel[3][3] = {
		{-1, 0, 0},
		{0, 0, 0},
		{0, 0, 1}
	};

	convolve(pDoc->inputimg, pDoc->resultimg, pDoc->ImageWidth, pDoc->ImageHeight, kernel, 128, pDoc->depth);

	Invalidate();
}


void Cimageproc20190854View::OnRegionPrewit()
{
	Cimageproc20190854Doc* pDoc = GetDocument();
	int i, x, y, value;
	int rvalue, gvalue, bvalue;
	float kernel_h[3][3] = {
		{-1, -1, -1},
		{0, 0, 0},
		{1, 1, 1}
	};
	float kernel_v[3][3] = {
		{-1, 0, 1},
		{-1, 0, 1},
		{-1, 0, 1}
	};
	unsigned char ** Er, ** Ec;
	Er = (unsigned char**)malloc(pDoc->ImageHeight * sizeof(unsigned char*));
	Ec = (unsigned char**)malloc(pDoc->ImageHeight * sizeof(unsigned char*));

	for (i = 0; i < pDoc->ImageHeight; i++) {
		Er[i] = (unsigned char*)malloc(pDoc->ImageWidth * pDoc->depth);
		Ec[i] = (unsigned char*)malloc(pDoc->ImageWidth * pDoc->depth);


}
		

	convolve(pDoc->inputimg,Er,  pDoc->ImageWidth, pDoc->ImageHeight, kernel_h, 128, pDoc->depth);
	convolve(pDoc->inputimg,Ec,  pDoc->ImageWidth, pDoc->ImageHeight, kernel_v,128 , pDoc->depth);

	for (y = 0; y < pDoc->ImageHeight; y++) 
		for (x = 0; x < pDoc->ImageWidth; x++) {
			if (pDoc->depth == 1) {
				value = sqrt((Er[y][x] - 128) * (Er[y][x] - 128) + (Ec[y][x] - 128) * (Ec[y][x] - 128));
				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][x] = (unsigned char)value;
			}
			else {
				rvalue = sqrt((Er[y][3 * x + 0] - 128) * (Er[y][3 * x + 0] - 128) + (Ec[y][3 * x + 0] - 128) * (Ec[y][3 * x + 0] - 128));;
				gvalue = sqrt((Er[y][3 * x + 1] - 128) * (Er[y][3 * x + 1] - 128) + (Ec[y][3 * x + 1] - 128) * (Ec[y][3 * x + 1] - 128));
				bvalue = sqrt((Er[y][3 * x + 2] - 128) * (Er[y][3 * x + 2] - 128) + (Ec[y][3 * x + 2] - 128) * (Ec[y][3 * x + 2] - 128));

				value = sqrt(rvalue * rvalue + gvalue * gvalue + bvalue * bvalue);

				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][3 * x + 0] = value;
				pDoc->resultimg[y][3 * x + 1] = value;
				pDoc->resultimg[y][3 * x + 2] = value;

				
			}
		}
	//메모리삭제
	for (i = 0; i < pDoc->ImageHeight; i++)
	{
		free(Er[i]);
		free(Ec[i]);
	}
	free(Er);
	free(Ec);

	Invalidate();

}



void Cimageproc20190854View::OnRegionRoberts()
{
	Cimageproc20190854Doc* pDoc = GetDocument();
	int i, x, y, value;
	int rvalue, gvalue, bvalue;
	float kernel_h[3][3] = {
		{-1, 0, 0},
		{0, 1, 0},
		{0, 0, 0}
	};
	float kernel_v[3][3] = {
		{0, 0, -1},
		{0, 1, 0},
		{0, 0,0}
	};
	unsigned char** Er, ** Ec;
	Er = (unsigned char**)malloc(pDoc->ImageHeight * sizeof(unsigned char*));
	Ec = (unsigned char**)malloc(pDoc->ImageHeight * sizeof(unsigned char*));

	for (i = 0; i < pDoc->ImageHeight; i++) {
		Er[i] = (unsigned char*)malloc(pDoc->ImageWidth * pDoc->depth);
		Ec[i] = (unsigned char*)malloc(pDoc->ImageWidth * pDoc->depth);


	}


	convolve(pDoc->inputimg, Er, pDoc->ImageWidth, pDoc->ImageHeight, kernel_h, 128, pDoc->depth);
	convolve(pDoc->inputimg, Ec, pDoc->ImageWidth, pDoc->ImageHeight, kernel_v, 128, pDoc->depth);

	for (y = 0; y < pDoc->ImageHeight; y++)
		for (x = 0; x < pDoc->ImageWidth; x++) {
			if (pDoc->depth == 1) {
				value = sqrt((Er[y][x] - 128) * (Er[y][x] - 128) + (Ec[y][x] - 128) * (Ec[y][x] - 128));
				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][x] = (unsigned char)value;
			}
			else {
				rvalue = sqrt((Er[y][3 * x + 0] - 128) * (Er[y][3 * x + 0] - 128) + (Ec[y][3 * x + 0] - 128) * (Ec[y][3 * x + 0] - 128));;
				gvalue = sqrt((Er[y][3 * x + 1] - 128) * (Er[y][3 * x + 1] - 128) + (Ec[y][3 * x + 1] - 128) * (Ec[y][3 * x + 1] - 128));
				bvalue = sqrt((Er[y][3 * x + 2] - 128) * (Er[y][3 * x + 2] - 128) + (Ec[y][3 * x + 2] - 128) * (Ec[y][3 * x + 2] - 128));

				value = sqrt(rvalue * rvalue + gvalue * gvalue + bvalue * bvalue);

				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][3 * x + 0] = value;
				pDoc->resultimg[y][3 * x + 1] = value;
				pDoc->resultimg[y][3 * x + 2] = value;


			}
		}
	//메모리삭제
	for (i = 0; i < pDoc->ImageHeight; i++)
	{
		free(Er[i]);
		free(Ec[i]);
	}
	free(Er);
	free(Ec);

	Invalidate();
}


void Cimageproc20190854View::OnRegionSobel()
{
	Cimageproc20190854Doc* pDoc = GetDocument();
	int i, x, y, value;
	int rvalue, gvalue, bvalue;
	float kernel_h[3][3] = {
		{-1, -2, -1},
		{0, 0, 0},
		{1, 2, 1}
	};
	float kernel_v[3][3] = {
		{1, 0, -1},
		{2, 0, -2},
		{1, 0, -1}
	};
	unsigned char** Er, ** Ec;
	Er = (unsigned char**)malloc(pDoc->ImageHeight * sizeof(unsigned char*));
	Ec = (unsigned char**)malloc(pDoc->ImageHeight * sizeof(unsigned char*));

	for (i = 0; i < pDoc->ImageHeight; i++) {
		Er[i] = (unsigned char*)malloc(pDoc->ImageWidth * pDoc->depth);
		Ec[i] = (unsigned char*)malloc(pDoc->ImageWidth * pDoc->depth);


	}


	convolve(pDoc->inputimg, Er, pDoc->ImageWidth, pDoc->ImageHeight, kernel_h, 128, pDoc->depth);
	convolve(pDoc->inputimg, Ec, pDoc->ImageWidth, pDoc->ImageHeight, kernel_v, 128, pDoc->depth);

	for (y = 0; y < pDoc->ImageHeight; y++)
		for (x = 0; x < pDoc->ImageWidth; x++) {
			if (pDoc->depth == 1) {
				value = sqrt((Er[y][x] - 128) * (Er[y][x] - 128) + (Ec[y][x] - 128) * (Ec[y][x] - 128));
				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][x] = (unsigned char)value;
			}
			else {
				rvalue = sqrt((Er[y][3 * x + 0] - 128) * (Er[y][3 * x + 0] - 128) + (Ec[y][3 * x + 0] - 128) * (Ec[y][3 * x + 0] - 128));;
				gvalue = sqrt((Er[y][3 * x + 1] - 128) * (Er[y][3 * x + 1] - 128) + (Ec[y][3 * x + 1] - 128) * (Ec[y][3 * x + 1] - 128));
				bvalue = sqrt((Er[y][3 * x + 2] - 128) * (Er[y][3 * x + 2] - 128) + (Ec[y][3 * x + 2] - 128) * (Ec[y][3 * x + 2] - 128));

				value = sqrt(rvalue * rvalue + gvalue * gvalue + bvalue * bvalue);

				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][3 * x + 0] = value;
				pDoc->resultimg[y][3 * x + 1] = value;
				pDoc->resultimg[y][3 * x + 2] = value;


			}
		}
	//메모리삭제
	for (i = 0; i < pDoc->ImageHeight; i++)
	{
		free(Er[i]);
		free(Ec[i]);
	}
	free(Er);
	free(Ec);

	Invalidate();
}
void Cimageproc20190854View::OnRegionEverageFlitering()
{
	Cimageproc20190854Doc* pDoc = GetDocument();
	int x, y, i, j;
	int sum;
	int pixelcount;
	int xpos, ypos;
	int rsum, gsum, bsum;


	for (y = 0; y < pDoc->ImageHeight; y++)
		for (x = 0; x < pDoc->ImageWidth; x++)
		{
			sum = 0;	pixelcount = 0; rsum = gsum = bsum=0;
			for (j = -2; j <= 2; j++)
				for (i = -2; i <= 2; i++)
				{
					xpos = x + i;
					ypos = y + j;
					if (xpos >= 0 && xpos <= pDoc->ImageWidth - 1 && ypos >= 0 && ypos <= pDoc->ImageHeight - 1)
					{
						if(pDoc->depth==1)
						sum += pDoc->inputimg[y + j][x + i];
						else {
							rsum += pDoc->inputimg[y + j][3 * (x + i) + 0];
							gsum += pDoc->inputimg[y + j][3 * (x +i) +1 ];
							bsum += pDoc->inputimg[y + j][3 * (x + i) + 2];
						}
						pixelcount++;
					}
				}
			if(pDoc->depth==1)pDoc->resultimg[y][x] = sum / pixelcount;
			else 
			{
				pDoc->resultimg[y][3 * x + 0] = rsum / pixelcount;
				pDoc->resultimg[y][3 * x + 1 ]= gsum / pixelcount;
				pDoc->resultimg[y][3 * x + 2 ]= bsum / pixelcount;
			}
			
		}
	Invalidate();
}


void Cimageproc20190854View::OnRegionMedianfiltering()
{
	Cimageproc20190854Doc* pDoc = GetDocument();
	int x, y, i, j, temp;
	int n[9];
	for (y = 1; y < pDoc->ImageHeight - 1; y++)
		for (x = 1; x < pDoc->ImageWidth - 1; x++) {
			if (pDoc->depth == 1) {


				n[0] = pDoc->inputimg[y - 1][x - 1];
				n[1] = pDoc->inputimg[y - 1][x];
				n[2] = pDoc->inputimg[y - 1][x + 1];
				n[3] = pDoc->inputimg[y][x - 1];
				n[4] = pDoc->inputimg[y][x];
				n[5] = pDoc->inputimg[y][x + 1];
				n[6] = pDoc->inputimg[y + 1][x - 1];
				n[7] = pDoc->inputimg[y + 1][x];
				n[8] = pDoc->inputimg[y + 1][x + 1];
				//버블정렬 (오름차순)
				for (i = 8; i > 0; i--) {
					for (j = 0; j < 8; j++) {
						if (n[j] > n[j + 1]) {

							temp = n[j + 1];
							n[j + 1] = n[j];
							n[j] = temp;

						}
					}
				}

				pDoc->resultimg[y][x] = n[4];
			}
			else {
				n[0] = pDoc->inputimg[y - 1][3*(x - 1)+0];
				n[1] = pDoc->inputimg[y - 1][3*x+0];
				n[2] = pDoc->inputimg[y - 1][3*(x + 1)+0];
				n[3] = pDoc->inputimg[y][3 * (x - 1) + 0];
				n[4] = pDoc->inputimg[y][3 * x + 0];
				n[5] = pDoc->inputimg[y][3 * (x + 1) + 0];
				n[6] = pDoc->inputimg[y + 1][3 * (x - 1) + 0];
				n[7] = pDoc->inputimg[y + 1][3 * x + 0];
				n[8] = pDoc->inputimg[y + 1][3 * (x + 1) + 0];

				for (i = 8; i > 0; i--) {
					for (j = 0; j < 8; j++) {
						if (n[j] > n[j + 1]) {
							temp = n[j + 1];
							n[j + 1] = n[j];
							n[j] = temp; 
						}
					}
				}
				pDoc->resultimg[y][3 * x + 0]=n[4];


				n[0] = pDoc->inputimg[y - 1][3 * (x - 1) + 1];
				n[1] = pDoc->inputimg[y - 1][3 * x + 1];
				n[2] = pDoc->inputimg[y - 1][3 * (x + 1) + 1];
				n[3] = pDoc->inputimg[y][3 * (x - 1) + 1];
				n[4] = pDoc->inputimg[y][3 * x + 1];
				n[5] = pDoc->inputimg[y][3 * (x + 1) + 1];
				n[6] = pDoc->inputimg[y + 1][3 * (x - 1) + 1];
				n[7] = pDoc->inputimg[y + 1][3 * x + 1];
				n[8] = pDoc->inputimg[y + 1][3 * (x + 1) + 1];

				for (i = 8; i > 0; i--) {
					for (j = 0; j < 8; j++) {
						if (n[j] > n[j + 1]) {
							temp = n[j + 1];
							n[j + 1] = n[j];
							n[j] = temp;
						}
					}
				}
				pDoc->resultimg[y][3 * x + 1] = n[4];


				n[0] = pDoc->inputimg[y - 1][3 * (x - 1) + 2];
				n[1] = pDoc->inputimg[y - 1][3 * x + 2];
				n[2] = pDoc->inputimg[y - 1][3 * (x + 1) + 2];
				n[3] = pDoc->inputimg[y][3 * (x - 1) + 2];
				n[4] = pDoc->inputimg[y][3 * x + 2];
				n[5] = pDoc->inputimg[y][3 * (x + 1) + 2];
				n[6] = pDoc->inputimg[y + 1][3 * (x - 1) + 2];
				n[7] = pDoc->inputimg[y + 1][3 * x + 2];
				n[8] = pDoc->inputimg[y + 1][3 * (x + 1) + 2];

				for (i = 8; i > 0; i--) {
					for (j = 0; j < 8; j++) {
						if (n[j] > n[j + 1]) {
							temp = n[j + 1];
							n[j + 1] = n[j];
							n[j] = temp;
						}
					}
				}
				pDoc->resultimg[y][3 * x + 2] = n[4];
			}
			Invalidate();
		}
}

void Cimageproc20190854View::OnMopologycolortogray()
{
	Cimageproc20190854Doc* pDoc = GetDocument();
	if (pDoc->depth == 1) return;

	int x, y;
	int gray;

	for (y = 1; y < pDoc->ImageHeight - 1; y++) {
		for (x = 1; x < pDoc->ImageWidth - 1; x++) {
			gray = ((pDoc->inputimg[y][x * 3 + 0] + pDoc->inputimg[y][x * 3 + 1] + pDoc->inputimg[y][x * 3 + 0]) / 3);
			pDoc->inputimg[y][x * 3 + 0] = gray;
			pDoc->inputimg[y][x * 3 + 1] = gray;
			pDoc->inputimg[y][x * 3 + 2] = gray;
		}
	}

	Invalidate();
}


void Cimageproc20190854View::OnMopologybinarization()
{
	Cimageproc20190854Doc* pDoc = GetDocument();
	int x, y;
	int th = 100;

	for (y = 1; y < pDoc->ImageHeight - 1; y++) {
		for (x = 1; x < pDoc->ImageWidth - 1; x++) {
			if (pDoc->depth == 1) {
				if (pDoc->inputimg[y][x] > th)
					pDoc->inputimg[y][x] = 255;
				else
					pDoc->inputimg[y][x] = 0;
			}
			else {
				if (pDoc->inputimg[y][x * 3 + 0] > th) {
					pDoc->inputimg[y][x * 3 + 0] = 255; 
					pDoc->inputimg[y][x * 3 + 1] = 255;
					pDoc->inputimg[y][x * 3 + 2] = 255;
				}
				else {
					pDoc->inputimg[y][x * 3 + 0] = 0;
					pDoc->inputimg[y][x * 3 + 1] = 0;
					pDoc->inputimg[y][x * 3 + 2] = 0;
				}
			}
		}
	}

	Invalidate();
}


void Cimageproc20190854View::OnMopologyerosion()
{
	Cimageproc20190854Doc* pDoc = GetDocument();
	int x, y;
	int min;

	for (y = 1; y < pDoc->ImageHeight - 1; y++) {
		for (x = 1; x < pDoc->ImageWidth - 1; x++) {
			if (pDoc->depth == 1) {	//흑백
				min = 255;
				for (int j = 0; j < 3; j++) {
					for (int i = 0; i < 3; i++) {
						if (min > pDoc->inputimg[y + j - 1][x + i - 1])
							min = pDoc->inputimg[y + j - 1][x + i - 1];
					}
				}
				pDoc->resultimg[y][x] = min;
			}
			else {	//컬러
				min = 255;
				for (int j = 0; j < 3; j++) {
					for (int i = 0; i < 3; i++) {
						if (min > pDoc->inputimg[y + j - 1][(x + i - 1) * 3 + 0])
							min = pDoc->inputimg[y + j - 1][(x + i - 1) * 3 + 0];
					}
				}
				pDoc->resultimg[y][x * 3 + 0] = min;

				min = 255;
				for (int j = 0; j < 3; j++) {
					for (int i = 0; i < 3; i++) {
						if (min > pDoc->inputimg[y + j - 1][(x + i - 1) * 3 + 1])
							min = pDoc->inputimg[y + j - 1][(x + i - 1) * 3 + 1];
					}
				}
				pDoc->resultimg[y][x * 3 + 1] = min;

				min = 255;
				for (int j = 0; j < 3; j++) {
					for (int i = 0; i < 3; i++) {
						if (min > pDoc->inputimg[y + j - 1][(x + i - 1) * 3 + 2])
							min = pDoc->inputimg[y + j - 1][(x + i - 1) * 3 + 2];
					}
				}
				pDoc->resultimg[y][x * 3 + 2] = min;
			}
		}
	}

	Invalidate();
}


void Cimageproc20190854View::OnMopologydilation()
{
	Cimageproc20190854Doc* pDoc = GetDocument();
	int x, y;
	int max;

	for (y = 1; y < pDoc->ImageHeight - 1; y++) {
		for (x = 1; x < pDoc->ImageWidth - 1; x++) {
			if (pDoc->depth == 1) {	//흑백
				max = 0;
				for (int j = 0; j < 3; j++) {
					for (int i = 0; i < 3; i++) {
						if (max < pDoc->inputimg[y + j - 1][x + i - 1])
							max = pDoc->inputimg[y + j - 1][x + i - 1];
					}
				}
				pDoc->resultimg[y][x] = max;
			}
			else {	//컬러
				max = 0;
				for (int j = 0; j < 3; j++) {
					for (int i = 0; i < 3; i++) {
						if (max < pDoc->inputimg[y + j - 1][(x + i - 1) * 3 + 0])
							max = pDoc->inputimg[y + j - 1][(x + i - 1) * 3 + 0];
					}
				}
				pDoc->resultimg[y][x * 3 + 0] = max;

				max = 0;
				for (int j = 0; j < 3; j++) {
					for (int i = 0; i < 3; i++) {
						if (max < pDoc->inputimg[y + j - 1][(x + i - 1) * 3 + 1])
							max = pDoc->inputimg[y + j - 1][(x + i - 1) * 3 + 1];
					}
				}
				pDoc->resultimg[y][x * 3 + 1] = max;

				max = 0;
				for (int j = 0; j < 3; j++) {
					for (int i = 0; i < 3; i++) {
						if (max < pDoc->inputimg[y + j - 1][(x + i - 1) * 3 + 2])
							max = pDoc->inputimg[y + j - 1][(x + i - 1) * 3 + 2];
					}
				}
				pDoc->resultimg[y][x * 3 + 2] = max;
			}
		}
	}

	Invalidate();
}
void Cimageproc20190854View::CopyResultToInput()
{
	Cimageproc20190854Doc* pDoc = GetDocument();
	int x, y;

	for (y = 1; y < pDoc->ImageHeight - 1; y++) {
		for (x = 1; x < pDoc->ImageWidth * pDoc->depth; x++) {
			pDoc->inputimg[y][x] = pDoc->resultimg[y][x];
		}
	}
}

void Cimageproc20190854View::OnMopologyopenning()
{
	OnMopologyerosion();
	CopyResultToInput();
	OnMopologyerosion();
	CopyResultToInput();
	OnMopologyerosion();
	CopyResultToInput();

	OnMopologydilation();
	CopyResultToInput();
	OnMopologydilation();
	CopyResultToInput();
	OnMopologydilation();
	CopyResultToInput();
}


void Cimageproc20190854View::OnMopologyclosing()
{
	OnMopologydilation();
	CopyResultToInput();
	OnMopologydilation();
	CopyResultToInput();
	OnMopologydilation();
	CopyResultToInput();

	OnMopologyerosion();
	CopyResultToInput();
	OnMopologyerosion();
	CopyResultToInput();
	OnMopologyerosion();
	CopyResultToInput();
}
void Cimageproc20190854View::OnGeometryZoominPixelCopy()
{
	Cimageproc20190854Doc* pDoc = GetDocument();
	int x, y, i, j;

	int xscale = 3;
	int yscale = 2;

	if (pDoc->gresultimg != NULL) {
		for (i = 0; i < pDoc->gImageHeight; i++) free(pDoc->gresultimg[i]);
		free(pDoc->gresultimg);
	}

	pDoc->gImageWidth = pDoc->ImageWidth * xscale;
	pDoc->gImageHeight = pDoc->ImageHeight * yscale;

	//메모리 할당
	pDoc->gresultimg = (unsigned char**)malloc(pDoc->gImageHeight * sizeof(unsigned char*));
	for (i = 0; i < pDoc->gImageHeight; i++) {
		pDoc->gresultimg[i] = (unsigned char*)malloc(pDoc->gImageWidth * pDoc->depth);
	}

	/*
	//전방향사상
	for (y = 0; y < pDoc->imageHeight; y++) {
		for (x = 0; x < pDoc->imageWidth; x++) {
			for (j = 0; j < yscale; j++) {
				for (i = 0; i < xscale; i++) {
					if (pDoc->depth == 1) {
						pDoc->gResultImage[yscale * y + j][xscale * x + i] = pDoc->inputImage[y][x];
					}
					else {
						pDoc->gResultImage[yscale * y + j][(xscale * x + i) * 3 + 0] = pDoc->inputImage[y][x * 3 + 0];
						pDoc->gResultImage[yscale * y + j][(xscale * x + i) * 3 + 1] = pDoc->inputImage[y][x * 3 + 1];
						pDoc->gResultImage[yscale * y + j][(xscale * x + i) * 3 + 2] = pDoc->inputImage[y][x * 3 + 2];
					}
				}
			}
		}
	}
	*/

	//역방향 사상
	for (y = 0; y < pDoc->gImageHeight; y++) {
		for (x = 0; x < pDoc->gImageWidth; x++) {
			if (pDoc->depth == 1) {
				pDoc->gresultimg[y][x] = pDoc->inputimg[y / yscale][x / xscale];
			}
			else {
				pDoc->gresultimg[y][x * 3 + 0] = pDoc->inputimg[y / yscale][(x / xscale) * 3 + 0];
				pDoc->gresultimg[y][x * 3 + 1] = pDoc->inputimg[y / yscale][(x / xscale) * 3 + 1];
				pDoc->gresultimg[y][x * 3 + 2] = pDoc->inputimg[y / yscale][(x / xscale) * 3 + 2];
			}
		}
	}

	Invalidate();

}




void Cimageproc20190854View::OnGeometryZoominBinuerinterpolation()
{
	Cimageproc20190854Doc* pDoc = GetDocument();
	int x, y, i, j;

	float xscale = 2.3;	//3 . . .
	float yscale = 1.7;	//2 . . .
	float src_x, src_y;
	float alpha, beta;
	int Ax, Ay, Bx, By, Cx, Cy, Dx, Dy;
	int E, F;

	if (pDoc->gresultimg != NULL) {
		for (i = 0; i < pDoc->gImageHeight; i++)
			free(pDoc->gresultimg[i]);
		free(pDoc->gresultimg);
	}

	pDoc->gImageWidth = pDoc->ImageWidth * xscale;
	pDoc->gImageHeight = pDoc->ImageHeight * yscale;
	//메모리 할당
	pDoc->gresultimg = (unsigned char**)malloc(pDoc->gImageHeight * sizeof(unsigned char*));
	for (i = 0; i < pDoc->gImageHeight; i++) {
		pDoc->gresultimg[i] = (unsigned char*)malloc(pDoc->gImageWidth * pDoc->depth);
	}

	//역방향 사상 (여기서 전방향 사상은 안됨. 중간중간 연산이 안될 수 있음)
	for (y = 0; y < pDoc->gImageHeight; y++) {
		for (x = 0; x < pDoc->gImageWidth; x++) {
			src_x = x / xscale;
			src_y = y / yscale;

			/*alpha = src_x - (int)src_x;
			beta = src_y - (int)src_y;*/
			alpha = src_x - src_x;
			beta = src_y - src_y;

			/*Ax = (int)src_x;
			Ay = (int)src_y;*/
			Ax = src_x;
			Ay = src_y;
			Bx = Ax + 1;
			By = Ay;
			Cx = Ax;
			Cy = Ay + 1;
			Dx = Ax + 1;
			Dy = Ax + 1;

			if (Bx > pDoc->ImageWidth - 1) Bx = pDoc->ImageWidth - 1;
			if (Cy > pDoc->ImageHeight - 1) Cy = pDoc->ImageHeight - 1;
			if (Dx > pDoc->ImageWidth - 1) Dx = pDoc->ImageWidth - 1;
			if (Dy > pDoc->ImageHeight - 1) Dy = pDoc->ImageHeight - 1;

			if (pDoc->depth == 1) {
				E = (1 - alpha) * pDoc->inputimg[Ay][Ax] + alpha * pDoc->inputimg[By][Bx];
				F = (1 - alpha) * pDoc->inputimg[Cy][Cx] + alpha * pDoc->inputimg[Dy][Dx];
				pDoc->gresultimg[y][x] = (1 - beta) * E + beta * F;
			}
			else {
				E = (1 - alpha) * pDoc->inputimg[Ay][3 * Ax  + 0] + alpha * pDoc->inputimg[By][3 * Bx  + 0];
				F = (1 - alpha) * pDoc->inputimg[Cy][3 * Cx  + 0] + alpha * pDoc->inputimg[Dy][3 * Dx  + 0];
				pDoc->gresultimg[y][3 * x + 0] = (1 - beta) * E + beta * F;

				E = (1 - alpha) * pDoc->inputimg[Ay][3 * Ax  + 1] + alpha * pDoc->inputimg[By][3 * Bx  + 1];
				F = (1 - alpha) * pDoc->inputimg[Cy][3 * Cx  + 1] + alpha * pDoc->inputimg[Dy][3 * Dx  + 1];
				pDoc->gresultimg[y][3 * x + 1] = (1 - beta) * E + beta * F;

				E = (1 - alpha) * pDoc->inputimg[Ay][3 * Ax  + 2] + alpha * pDoc->inputimg[By][3 * Bx  + 2];
				F = (1 - alpha) * pDoc->inputimg[Cy][3 * Cx  + 2] + alpha * pDoc->inputimg[Dy][3 * Dx  + 2];
				pDoc->gresultimg[y][3 * x + 2] = (1 - beta) * E + beta * F;
			}
		}
	}

	Invalidate();
}



void Cimageproc20190854View::OnGeometryZoomoutSubsampling()
{
	Cimageproc20190854Doc* pDoc = GetDocument();
	int x, y, i;

	int xscale = 3; // 1/3
	int yscale = 2; // 1/2

	if (pDoc->gresultimg != NULL) {
		for (i = 0; i < pDoc->gImageHeight; i++) free(pDoc->gresultimg[i]);
		free(pDoc->gresultimg);
	}
	pDoc->gImageWidth = (pDoc->ImageWidth) / xscale;
	pDoc->gImageHeight = (pDoc->ImageHeight) / yscale;


	//메모리 할당
	pDoc->gresultimg = (unsigned char**)malloc(pDoc->gImageHeight * sizeof(unsigned char*));
	for (i = 0; i < pDoc->gImageHeight; i++) {
		pDoc->gresultimg[i] = (unsigned char*)malloc(pDoc->gImageWidth * pDoc->depth);
	}


	for (y = 0; y < pDoc->gImageHeight; y++) {
		for (x = 0; x < pDoc->gImageWidth; x++)
		{
			if (pDoc->depth == 1)
				pDoc->gresultimg[y][x] = pDoc->inputimg[y * yscale][x * xscale];
			else {
				pDoc->gresultimg[y][3 * x + 0] = pDoc->inputimg[y * yscale][3 * (x * xscale) + 0];
				pDoc->gresultimg[y][3 * x + 1] = pDoc->inputimg[y * yscale][3 * (x * xscale) + 1];
				pDoc->gresultimg[y][3 * x + 2] = pDoc->inputimg[y * yscale][3 * (x * xscale) + 2];

			}
		}
		Invalidate();

	}
}


	void Cimageproc20190854View::OnGeometryZoomoutMeanSub()
	{
		OnRegionSmoothing();
		CopyResultToInput();
		OnGeometryZoomoutSubsampling();
	}


	void Cimageproc20190854View::OnGeometryZoomoutAvg()
	{
		Cimageproc20190854Doc* pDoc = GetDocument();
		int x, y, i, j;
		int src_x, src_y;
		int sum;
		int rsum, gsum, bsum;

		int xscale = 3;
		int yscale = 2;

		if (pDoc->gresultimg != NULL) {
			for (i = 0; i < pDoc->gImageHeight; i++) free(pDoc->gresultimg[i]);
			free(pDoc->gresultimg);
		}

		pDoc->gImageWidth = pDoc->ImageWidth / xscale;
		pDoc->gImageHeight = pDoc->ImageHeight / yscale;
		//메모리 할당
		pDoc->gresultimg = (unsigned char**)malloc(pDoc->gImageHeight * sizeof(unsigned char*));
		for (i = 0; i < pDoc->gImageHeight; i++) {
			pDoc->gresultimg[i] = (unsigned char*)malloc(pDoc->gImageWidth * pDoc->depth);
		}

		//전방향 사상
		for (y = 0; y < pDoc->ImageHeight - yscale; y += yscale) {
			for (x = 0; x < pDoc->ImageWidth - xscale; x += xscale) {
				if (pDoc->depth == 1) {
					sum = 0;
					for (j = 0; j < yscale; j++) {
						for (i = 0; i < xscale; i++) {
							src_x = x + i;
							src_y = y + j;
							sum += pDoc->inputimg[src_y][src_x];
						}
					}
					pDoc->gresultimg[y / yscale][x / xscale] = sum / (xscale * yscale);
				}
				else {
					rsum = gsum = bsum = 0;
					for (j = 0; j < yscale; j++) {
						for (i = 0; i < xscale; i++) {
							src_x = x + i;
							src_y = y + j;
							rsum += pDoc->inputimg[src_y][3 * src_x + 0];
							gsum += pDoc->inputimg[src_y][3 * src_x + 1];
							bsum += pDoc->inputimg[src_y][3 * src_x + 2];
						}
					}
					pDoc->gresultimg[y / yscale][3 * (x / xscale)  + 0] = rsum / (xscale * yscale);
					pDoc->gresultimg[y / yscale][3 * (x / xscale)  + 1] = gsum / (xscale * yscale);
					pDoc->gresultimg[y / yscale][3 * (x / xscale)  + 2] = bsum / (xscale * yscale);
				}
			}
		}

		Invalidate();
	
	}

#define PI 3.1415926521
	void Cimageproc20190854View::OnGeometryRotation()
	{
		Cimageproc20190854Doc* pDoc = GetDocument();

		CAngleinputDialog dlg;


		int angle = 0; //degree
		float radian;
		int x, y, i;
		int Cx, Cy, Hy;
		int x_source, y_source;
		int xdiff, ydiff;

		dlg.m_iAngle = angle;
		if (dlg.DoModal() == IDCANCEL) return;
		angle = dlg.m_iAngle;

		radian = PI / 180 * angle;

		if (pDoc->gresultimg != NULL) {
			for (i = 0; i < pDoc->gImageHeight; i++)
				free(pDoc->gresultimg[i]);
			free(pDoc->gresultimg);
		}

		pDoc->gImageWidth = pDoc->ImageHeight * fabs(cos(PI / 2 - radian)) + pDoc->ImageWidth * fabs(cos(radian));
		pDoc->gImageHeight = pDoc->ImageHeight * fabs(cos(radian)) + pDoc->ImageWidth * fabs(cos(PI / 2 - radian));
		//메모리 할당
		pDoc->gresultimg = (unsigned char**)malloc(pDoc->gImageHeight * sizeof(unsigned char*));
		for (i = 0; i < pDoc->gImageHeight; i++) {
			pDoc->gresultimg[i] = (unsigned char*)malloc(pDoc->gImageWidth * pDoc->depth);
		}

		//중심점
		Cx = pDoc->ImageWidth / 2;
		Cy = pDoc->ImageHeight / 2;
		//y의 마지막좌표
		Hy = pDoc->ImageHeight - 1;

		xdiff = (pDoc->gImageWidth - pDoc->ImageWidth) / 2;
		ydiff = (pDoc->gImageHeight - pDoc->ImageHeight) / 2;

		for (y = -ydiff; y < pDoc->gImageHeight - ydiff; y++) {
			for (x = -xdiff; x < pDoc->gImageWidth - xdiff; x++) {

				x_source = ((Hy - y) - Cy) * sin(radian) + (x - Cx) * cos(radian) + Cx;
				y_source = Hy - (((Hy - y) - Cy) * cos(radian) - (x - Cx) * sin(radian) + Cy);

				if (pDoc->depth == 1) {
					if (x_source < 0 || x_source > pDoc->ImageWidth - 1 || y_source < 0 || y_source > pDoc->ImageHeight - 1) {
						pDoc->gresultimg[y + ydiff][x + xdiff] = 255;
					}
					else {
						pDoc->gresultimg[y + ydiff][x + xdiff] = pDoc->inputimg[y_source][x_source];
					}
				}
				else {
					if (x_source < 0 || x_source > pDoc->ImageWidth - 1 || y_source < 0 || y_source > pDoc->ImageHeight - 1) {
						pDoc->gresultimg[y + ydiff][(x + xdiff) * 3 + 0] = 255;
						pDoc->gresultimg[y + ydiff][(x + xdiff) * 3 + 1] = 255;
						pDoc->gresultimg[y + ydiff][(x + xdiff) * 3 + 2] = 255;
					}
					else {
						pDoc->gresultimg[y + ydiff][(x + xdiff) * 3 + 0] = pDoc->inputimg[y_source][x_source * 3 + 0];
						pDoc->gresultimg[y + ydiff][(x + xdiff) * 3 + 1] = pDoc->inputimg[y_source][x_source * 3 + 1];
						pDoc->gresultimg[y + ydiff][(x + xdiff) * 3 + 2] = pDoc->inputimg[y_source][x_source * 3 + 2];
					}
				}
			}
		}

		Invalidate();
	}

		
	

	
	
	
	


	void Cimageproc20190854View::OnGeometryMirror()
	{
		Cimageproc20190854Doc* pDoc = GetDocument(); 

		int x, y;

		for (y = 0; y < pDoc->ImageHeight; y++) {
			for (x = 0; x < pDoc->ImageWidth; x++) {
				if (pDoc->depth == 1) {
					pDoc->resultimg[y][x] = pDoc->inputimg[y][pDoc->ImageWidth - 1 - x];

				}
				else {
					pDoc->resultimg[y][3 * x + 0] = pDoc->inputimg[y][3 * (pDoc->ImageWidth - 1 - x) + 0];
					pDoc->resultimg[y][3 * x + 1] = pDoc->inputimg[y][3 * (pDoc->ImageWidth - 1 - x) + 1];
					pDoc->resultimg[y][3 * x + 2] = pDoc->inputimg[y][3 * (pDoc->ImageWidth - 1 - x) + 2];
				}
			}
		}
				Invalidate();
			
		}
	void Cimageproc20190854View::OnGeometryFlip()
	
	{
		Cimageproc20190854Doc* pDoc = GetDocument(); 
		int x, y;

		for (y = 0; y < pDoc->ImageHeight; y++) {
			for (x = 0; x < pDoc->ImageWidth; x++) {
				if (pDoc->depth == 1) {
					pDoc->resultimg[y][x] = pDoc->inputimg[pDoc->ImageHeight-1-y][x]; 

				}
				else {
					pDoc->resultimg[y][3 * x + 0] = pDoc->inputimg[pDoc->ImageHeight - 1 - y][3 *x + 0];
					pDoc->resultimg[y][3 * x + 1] = pDoc->inputimg[pDoc->ImageHeight - 1 - y][3 * x+ 1];
					pDoc->resultimg[y][3 * x + 2] = pDoc->inputimg[pDoc->ImageHeight - 1 - y][3 * x + 2];
				}
			}
		}
		Invalidate();
	}

	typedef struct 
	{
		int Px;
		int Py;
		int Qx;
		int Qy;
	}control_line;


	/*void Cimageproc20190854View::OnGeometryWarrping()
	{
		Cimageproc20190854Doc* pDoc = GetDocument();

		control_line source;
	}*/

	control_line mctrl_source = { 100, 100, 150, 150 };
	control_line mctrl_dest = { 100, 100, 200, 200 };

	void Cimageproc20190854View::OnGeometryWarping()
	{
		Cimageproc20190854Doc* pDoc = GetDocument();

		control_line source_lines[5] = {
		{100, 100, 150, 150},
		{0, 0, pDoc->ImageWidth - 1, 0},
		{pDoc->ImageWidth - 1, 0, pDoc->ImageWidth - 1, pDoc->ImageHeight - 1},
		{pDoc->ImageWidth - 1, pDoc->ImageHeight - 1, 0, pDoc->ImageHeight - 1},
		{0, pDoc->ImageHeight - 1, 0, 0}
		};
		control_line dest_lines[5] = {
			{100, 100, 200, 200},
			{0, 0, pDoc->ImageWidth - 1, 0},
			{pDoc->ImageWidth - 1, 0, pDoc->ImageWidth - 1, pDoc->ImageHeight - 1},
			{pDoc->ImageWidth - 1, pDoc->ImageHeight - 1, 0, pDoc->ImageHeight - 1},
			{0, pDoc->ImageHeight - 1, 0, 0}
		};

		source_lines[0] = mctrl_source;
		dest_lines[0] = mctrl_dest;

		int x, y;

		double u;	// 수직 교차점 위치
		double h;	// 제어선으로부터 픽셀의 수직변위(수직거리)
		double d;	// 제어선과 픽셀 사이의 거리
		double tx, ty;	// 결과영상 픽셀에 대응되는 입력 영상 픽셀 사이의 변위의 합
		double xp, yp;	// 각 제어선에 대해 계산된 입력 영상의 대응되는 픽셀 위치
		double weight;	// 각 제어선의 가중치
		double totalWeight;	// 전체 가중치의 합
		double a = 0.001;
		double b = 2.0;
		double p = 0.75;

		int x1, y1, x2, y2;
		int src_x1, src_y1, src_x2, src_y2;
		double src_line_length, dest_line_length;

		int num_lines = 5;	//제어선의 수
		int line;
		int source_x, source_y;

		for (y = 0; y < pDoc->ImageHeight; y++) {
			for (x = 0; x < pDoc->ImageWidth; x++) {
				tx = 0.0;
				ty = 0.0;
				totalWeight = 0.0;

				for (line = 0; line < num_lines; line++) {
					x1 = dest_lines[line].Px;
					y1 = dest_lines[line].Py;
					x2 = dest_lines[line].Qx;
					y2 = dest_lines[line].Qy;

					dest_line_length = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

					u = (double)((x - x1) * (x2 - x1) + (y - y1) * (y2 - y1))
						/ (double)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
					h = ((y - y1) * (x2 - x1) - (x - x1) * (y2 - y1)) / dest_line_length;

					if (u < 0)
						d = sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));
					else if (u > 1)
						d = sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));
					else
						d = fabs(h);

					src_x1 = source_lines[line].Px;
					src_y1 = source_lines[line].Py;
					src_x2 = source_lines[line].Qx;
					src_y2 = source_lines[line].Qy;

					src_line_length = sqrt((src_x2 - src_x1) * (src_x2 - src_x1) + (src_y2 - src_y1) * (src_y2 - src_y1));
					xp = src_x1 + u * (src_x2 - src_x1) - h * (src_y2 - src_y1) / src_line_length;
					yp = src_y1 + u * (src_y2 - src_y1) + h * (src_x2 - src_x1) / src_line_length;

					weight = pow(pow(dest_line_length, p) / (a + d), b);

					tx += (xp - x) * weight;
					ty += (yp - y) * weight;
					totalWeight += weight;
				}

				source_x = x + tx / totalWeight;
				source_y = y + ty / totalWeight;

				if (source_x < 0) source_x = 0;
				else if (source_x > pDoc->ImageWidth - 1) source_x = pDoc->ImageWidth - 1;
				if (source_y < 0) source_y = 0;
				else if (source_y > pDoc->ImageHeight - 1) source_y = pDoc->ImageHeight - 1;

				if (pDoc->depth == 1) {
					pDoc->resultimg[y][x] = pDoc->inputimg[source_y][source_x];
				}
				else {
					pDoc->resultimg[y][3 * x + 0] = pDoc->inputimg[source_y][3 * source_x + 0];
					pDoc->resultimg[y][3 * x + 1] = pDoc->inputimg[source_y][3 * source_x  + 1];
					pDoc->resultimg[y][3 * x + 2] = pDoc->inputimg[source_y][3 * source_x  + 2];
				}
			}
		}

		Invalidate();
	}
	CPoint mpos_st, mpos_end;

     #define NUM_FRAMES 10 
	void Cimageproc20190854View::OnGeometryMorphing()
	{
		Cimageproc20190854Doc* pDoc = GetDocument();

		CFileDialog dlg(true); 
		CFile file;
		 
		control_line source_lines[23] = 
		{ {116,7,207,5},{34,109,90,21},{55,249,30,128},{118,320,65,261},
		 {123,321,171,321},{179,319,240,264},{247,251,282,135},{281,114,228,8},
		 {78,106,123,109},{187,115,235,114},{72,142,99,128},{74,150,122,154},
		 {108,127,123,146},{182,152,213,132},{183,159,229,157},{219,131,240,154},
		 {80,246,117,212},{127,222,146,223},{154,227,174,221},{228,252,183,213},
		 {114,255,186,257},{109,258,143,277},{152,278,190,262} };
		control_line dest_lines[23] = 
		{ {120,8,200,6},{12,93,96,16},{74,271,16,110},{126,336,96,290},
		 {142,337,181,335},{192,335,232,280},{244,259,288,108},{285,92,212,13},
		 {96,135,136,118},{194,119,223,125},{105,145,124,134},{110,146,138,151},
		 {131,133,139,146},{188,146,198,134},{189,153,218,146},{204,133,221,140},
		 {91,268,122,202},{149,206,159,209},{170,209,181,204},{235,265,208,199},
		 {121,280,205,284},{112,286,160,301},{166,301,214,287} };

		double u;          
		double h;       
		double d;       
		double tx, ty;    
		double xp, yp;      
		double weight;            
		double totalWeight;         
		double a = 0.001; 
		double b = 2.0; 
		double p = 0.75;
		unsigned char** warpedImg;
		unsigned char** warpedImg2; 
		int frame; 
		double fweight; 
		control_line warp_lines[23]; 
		double tx2, ty2, xp2, yp2; 
		int dest_x1, dest_y1, dest_x2, dest_y2, source_x2, source_y2; 
		int x1, x2, y1, y2, src_x1, src_y1, src_x2, src_y2; 
		double src_line_length, dest_line_length; 
		int i, j; 
		int num_lines = 23;          
		int line;
		int x, y;
		int source_x, source_y;
		int last_row, last_col; 

		LoadTwoImage();

		warpedImg = (unsigned char**)malloc(pDoc->ImageHeight * sizeof(unsigned char*));
		for (i = 0; i < pDoc->ImageHeight; i++) {
			warpedImg[i] = (unsigned char*)malloc(pDoc->ImageWidth * pDoc->depth); 
		}

		warpedImg2 = (unsigned char**)malloc(pDoc->ImageHeight * sizeof(unsigned char*));
		for (i = 0; i < pDoc->ImageHeight; i++) {
			warpedImg2[i] = (unsigned char*)malloc(pDoc->ImageWidth * pDoc->depth); 
		}
		for (i = 0; i < NUM_FRAMES; i++) {
			pDoc->morhedImg[i] = (unsigned char**)malloc(pDoc->ImageHeight * sizeof(unsigned char*));
			for (j = 0; j < pDoc->ImageHeight; j++) {
				pDoc->morhedImg[i][j] = (unsigned char*)malloc(pDoc->ImageWidth * pDoc->depth);
			}
		}
		last_row = pDoc->ImageHeight - 1;
		last_col = pDoc->ImageWidth - 1;
		for (frame = 1; frame <= NUM_FRAMES; frame++) 
		{
			// 중간 프레임에 대한 가중치 계산 
			fweight = (double)(frame) / NUM_FRAMES; 

			// 중간 프레임에 대한 제어선 계산 
			for (line = 0; line < num_lines; line++) 
			{
				warp_lines[line].Px = (int)(source_lines[line].Px +
					(dest_lines[line].Px - source_lines[line].Px) * fweight);
				warp_lines[line].Py = (int)(source_lines[line].Py +
					(dest_lines[line].Py - source_lines[line].Py) * fweight);
				warp_lines[line].Qx = (int)(source_lines[line].Qx +
					(dest_lines[line].Qx - source_lines[line].Qx) * fweight);
				warp_lines[line].Qy = (int)(source_lines[line].Qy +
					(dest_lines[line].Qy - source_lines[line].Qy) * fweight);
			}
			for (y = 0; y < pDoc->ImageHeight; y++)
			{
				for (x = 0; x < pDoc->ImageWidth; x++)
				{
					totalWeight = 0.0;
					tx = 0.0;
					ty = 0.0;
					tx2 = 0.0;
					ty2 = 0.0;
					// 각 제어선에 대하여 
					for (line = 0; line < num_lines; line++)
					{
						x1 = warp_lines[line].Px;
						y1 = warp_lines[line].Py;
						x2 = warp_lines[line].Qx;
						y2 = warp_lines[line].Qy;
						dest_line_length = sqrt((double)(x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

						// 수직교차점의 위치 및 픽셀의 수직 변위 계산 
						u = (double)((x - x1) * (x2 - x1) + (y - y1) * (y2 - y1)) /
							(double)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
						h = (double)((y - y1) * (x2 - x1) - (x - x1) * (y2 - y1)) / dest_line_length;

						// 제어선과 픽셀 사이의 거리 계산 
						if (u < 0) d = sqrt((double)(x - x1) * (x - x1) + (y - y1) * (y - y1));
						else if (u > 1) d = sqrt((double)(x - x2) * (x - x2) + (y - y2) * (y - y2));
						else d = fabs(h);

						src_x1 = source_lines[line].Px;
						src_y1 = source_lines[line].Py;
						src_x2 = source_lines[line].Qx;
						src_y2 = source_lines[line].Qy;
						src_line_length = sqrt((double)(src_x2 - src_x1) * (src_x2 - src_x1) +
							(src_y2 - src_y1) * (src_y2 - src_y1));

						dest_x1 = dest_lines[line].Px;
						dest_y1 = dest_lines[line].Py;
						dest_x2 = dest_lines[line].Qx;
						dest_y2 = dest_lines[line].Qy;
						dest_line_length = sqrt((double)(dest_x2 - dest_x1) * (dest_x2 - dest_x1) +
							(dest_y2 - dest_y1) * (dest_y2 - dest_y1));

						// 입력 영상 1에서의 대응 픽셀 위치 계산 
						xp = src_x1 + u * (src_x2 - src_x1) -
							h * (src_y2 - src_y1) / src_line_length;
						yp = src_y1 + u * (src_y2 - src_y1) +
							h * (src_x2 - src_x1) / src_line_length;

						// 입력 영상 2에서의 대응 픽셀 위치 계산 
						xp2 = dest_x1 + u * (dest_x2 - dest_x1) -
							h * (dest_y2 - dest_y1) / dest_line_length;
						yp2 = dest_y1 + u * (dest_y2 - dest_y1) +
							h * (dest_x2 - dest_x1) / dest_line_length;

						// 제어선에 대한 가중치 계산 
						weight = pow((pow((double)(dest_line_length), p) / (a + d)), b);

						// 입력 영상 1의 대응 픽셀과의 변위 계산 
						tx += (xp - x) * weight;
						ty += (yp - y) * weight;

						// 입력 영상 2의 대응 픽셀과의 변위 계산 
						tx2 += (xp2 - x) * weight;
						ty2 += (yp2 - y) * weight;

						totalWeight += weight;
					}

					// 입력 영상 1의 대응 픽셀 위치 계산     
					source_x = x + (int)(tx / totalWeight + 0.5);
					source_y = y + (int)(ty / totalWeight + 0.5);

					// 입력 영상 2의 대응 픽셀 위치 계산 
					source_x2 = x + (int)(tx2 / totalWeight + 0.5);
					source_y2 = y + (int)(ty2 / totalWeight + 0.5);

					// 영상의 경계를 벗어나는지 검사 
					if (source_x < 0) source_x = 0;
					if (source_x > last_col) source_x = last_col;
					if (source_y < 0) source_y = 0;
					if (source_y > last_row) source_y = last_row;

					if (source_x2 < 0) source_x2 = 0;
					if (source_x2 > last_col) source_x2 = last_col;
					if (source_y2 < 0) source_y2 = 0;
					if (source_y2 > last_row) source_y2 = last_row;

					// 워핑 결과 저장 
					warpedImg[y][x] = pDoc->inputimg[source_y][source_x];
					warpedImg2[y][x] = pDoc->inputimg2[source_y2][source_x2];
				}
			}

			// 모핑 결과 합병 
			for (y = 0; y < pDoc->ImageHeight; y++)
				for (x = 0; x < pDoc->ImageWidth; x++) {
					int val = (int)((1.0 - fweight) * warpedImg[y][x] +
						fweight * warpedImg2[y][x]);
					if (val < 0) val = 0;
					if (val > 255) val = 255;
					pDoc->morhedImg[frame - 1][y][x] = val;
				}
		}
		viewMode = MORPHING;  
		Invalidate(); 


	}


	void Cimageproc20190854View::OnLButtonDown(UINT nFlags, CPoint point)
	{
		mpos_st = point;


		CScrollView::OnLButtonDown(nFlags, point);
	}


	void Cimageproc20190854View::OnLButtonUp(UINT nFlags, CPoint point)
	{
		mpos_end = point;
		CDC* pDC = GetDC();
		CPen rpen;
		rpen.CreatePen(PS_SOLID,5,RGB(255,0,0));
		pDC->SelectObject(&rpen);


		pDC->MoveTo(mpos_st);
		pDC->LineTo(mpos_end);

		ReleaseDC(pDC);

		int Ax, Ay, Bx, By;
		Ax = mpos_st.x;
		Ay = mpos_st.y;
		Bx = mpos_end.x;
		By = mpos_end.y;


		if (Ax < Bx) mctrl_source.Px = Ax - (Bx - Ax) / 2;
		else			  mctrl_source.Px = Ax + (Ax - Bx) / 2;

		if (Ay < By) mctrl_source.Py = Ay - (By - Ay) / 2;
		else			  mctrl_source.Py = Ay + (Ay- By) / 2;

		mctrl_dest.Px = mctrl_source.Px;
		mctrl_dest.Py = mctrl_source.Py;

		mctrl_source.Qx = mpos_st.x;
		mctrl_source.Qy = mpos_st.y;
		mctrl_dest.Qx = mpos_end.x;
		mctrl_dest.Qy = mpos_end.y;




		

		


		CScrollView::OnLButtonUp(nFlags, point);
	}


	void Cimageproc20190854View::OnAviView()
	{
		Cimageproc20190854Doc* pDoc = GetDocument();

		CFileDialog dlg(true, "", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			"AVI파일(*.avi)|*.avi|모든파일|*.*|");

		if (dlg.DoModal() == IDOK)
		{
			AviFileName=dlg.GetPathName();
			bAviMode = true;
			Invalidate();
		}


	}


	void Cimageproc20190854View::LoadAviFile(CDC* pDC)
	{
		PAVIFILE pavi;
		AVIFILEINFO fi;
		int stm;
		PAVISTREAM pstm = NULL;
		AVISTREAMINFO si;
		PGETFRAME pfrm = NULL;
		int frame;
		LPBITMAPINFOHEADER pbmpih;
		unsigned char* image;
		int x, y;

		AVIFileInit();
		AVIFileOpen(&pavi, AviFileName,OF_READ|OF_SHARE_DENY_NONE,NULL);
		AVIFileInfo(pavi, &fi, sizeof(AVIFILEINFO));
		for (stm = 0; stm < fi.dwStreams; stm++) {
			AVIFileGetStream(pavi, &pstm, 0, stm);
			AVIStreamInfo(pstm, &si, sizeof(si));
			if (si.fccType == streamtypeVIDEO)
			{
				pfrm = AVIStreamGetFrameOpen(pstm, NULL);
				for (frame = 0; frame < si.dwLength; frame++) {
					pbmpih = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pfrm, frame);
					if (!pbmpih) continue;
					image = (unsigned char*)((LPSTR)pbmpih + (WORD)pbmpih->biSize);

					
					/*for (y = 0; y < fi.dwHeight; y++) {
						for (x = 0; x < fi.dwWidth; x++) {
							pDC->SetPixel(x, fi.dwHeight - y - 1, RGB(
								image[(y * fi.dwWidth + x) * 3 + 2]
								, image[(y * fi.dwWidth + x) * 3 + 1]
								, image[(y * fi.dwWidth + x) * 3 + 0]));
						}
					}*/
					
					pDC->SetStretchBltMode(COLORONCOLOR);
					::SetDIBitsToDevice(pDC->GetSafeHdc(),
						0, 0, fi.dwWidth, fi.dwHeight,
						0, 0, 0, fi.dwWidth,
						image, (BITMAPINFO*)pbmpih, DIB_RGB_COLORS);
					Sleep(20);
				}
			}
		}

		AVIStreamGetFrameClose(pfrm);
		AVIStreamRelease(pstm);
		AVIFileRelease(pavi);
		AVIFileExit();
	}

	
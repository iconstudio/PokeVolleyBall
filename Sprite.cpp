#include "pch.h"
#include "Sprite.h"


GameSprite::GameSprite(HINSTANCE instance, const UINT resource, const UINT number, const int xoff, const int yoff)
	: raw(CImage()), raw_width(0), raw_height(0), width(0), height(0), number(number), xoffset(xoff), yoffset(yoff) {
	raw.LoadFromResource(instance, resource);
	if (raw.IsNull()) {
		WCHAR temp[256];

		swprintf_s(temp, TEXT("자원 스프라이트 %u을(를) 불러오는 중에 오류가 발생했습니다."), resource);

		int error = MessageBox(NULL, temp, TEXT("오류"), MB_OK);

		if (error) {
			SendMessage(NULL, WM_CLOSE, 0, 0);
		}
	}
	
	raw.SetHasAlphaChannel(true);
	raw_width = raw.GetWidth();
	raw_height = raw.GetHeight();

	var result = __process_image(raw, raw_width, raw_height);
	if (!result) {
		WCHAR temp[256];

		swprintf_s(temp, TEXT("자원 스프라이트 %u이(가) 올바른 크기를 갖고 있지 않습니다."), resource);

		int error = MessageBox(NULL, temp, L"오류", MB_OK);
		if (error) {
			SendMessage(NULL, WM_CLOSE, 0, 0);
		}
	}
}

GameSprite::GameSprite(const LPCTSTR path, const UINT number, const int xoff, const int yoff)
	: raw(CImage()), raw_width(0), raw_height(0), width(0), height(0), number(number), xoffset(xoff), yoffset(yoff) {
	raw.Load(path);
	if (raw.IsNull()) {
		WCHAR temp[256];

		swprintf_s(temp, TEXT("경로 %s에서 스프라이트를 불러올 수 없습니다."), path);

		int error = MessageBox(NULL, temp, TEXT("오류"), MB_OK);

		if (error) {
			SendMessage(NULL, WM_CLOSE, 0, 0);
		}
	}

	raw.SetHasAlphaChannel(true);
	raw_width = raw.GetWidth();
	raw_height = raw.GetHeight();

	var result = __process_image(raw, raw_width, raw_height);
	if (!result) {
		WCHAR temp[256];

		swprintf_s(temp, TEXT("%s에 위치한 그림 파일이 올바른 크기를 갖고 있지 않습니다."), path);

		int error = MessageBox(NULL, temp, TEXT("오류"), MB_OK);
		if (error) {
			SendMessage(NULL, WM_CLOSE, 0, 0);
		}
	}
}

GameSprite::~GameSprite() {
	if (0 < number) {
		for (auto& image : frames) {
			image->Destroy();
		}
	}
	raw.Destroy();

	frames.clear();
}

void GameSprite::draw(HDC surface, const double dx, const double dy, const UINT index, const double angle, const double xscale, const double yscale) {
	if (1 < number) {
		
	} else {
		__draw_single(surface, raw, dx, dy, angle, xscale, yscale);
	}
}

void GameSprite::set_bbox(const LONG left, const LONG right, const LONG top, const LONG bottom) {
	bbox.left = left;
	bbox.right = right;
	bbox.top = top;
	bbox.bottom = bottom;
}

const int GameSprite::get_width() const {
	return width;
}

const int GameSprite::get_height() const {
	return height;
}

bool GameSprite::__process_image(CImage& image, const size_t width, const size_t height) {
	if (0 < width && 0 < height) {
		if (1 < number) { // 애니메이션을 위해서는 가로로 길쭉한 그림이 필요합니다.
			int slice_w, temp_w;
			if (1 == width) {
				slice_w = 1;
			} else {
				temp_w = (width / number);
				if (temp_w < 2) { // 0, 1
					slice_w = 1;
				} else {
					slice_w = temp_w;
				}
			}

			// 예약
			frames.reserve(number);

			auto raw_bitlevel = raw.GetBPP();

			for (UINT i = 0; i < number; ++i) { // 프레임 삽입
				// 1. 삽입할 그림 생성
				auto image_slice = new CImage(); // make_shared<CImage>();
				image_slice->Create(slice_w, height, raw_bitlevel);
				image_slice->SetHasAlphaChannel(true);

				auto slice_buffer = image_slice->GetDC();

				// 2. 원본 그림의 (i * slice_width, 0)에 위치한 내용을 조각 그림의 (0, 0) 위치에 복사
				raw.BitBlt(slice_buffer, 0, 0, slice_w, height, i * slice_w, 0, SRCCOPY);

				// 3. 메모리 최적화
				image_slice->ReleaseDC(); // slice_buffer 해제

				// 4. 낱장 삽입 (소유권 이전으로 이제 수정 불가)
				frames.emplace_back(image_slice);
			}
		} else { // one frame
			this->width = raw_width;
			this->height = raw_height;
		}

		set_bbox(-xoffset, -yoffset, this->width - xoffset, this->height - yoffset);
	} else { // failed!
		return false;
	}

	return true;
}

void GameSprite::__draw_single(HDC surface, CImage& image, const double dx, const double dy, const double angle, const double xscale, const double yscale) {
	if (0.0 != angle) {
		float cosine = (float)lengthdir_x(1, angle);
		float sine = (float)lengthdir_y(1, angle);

		int center_x = dx, center_y = dy;

		int nGraphicsMode = SetGraphicsMode(surface, GM_ADVANCED);

		// 실제와는 달리 y 좌표가 뒤집힘
		XFORM xform;
		xform.eM11 = cosine;
		xform.eM12 = sine;
		xform.eM21 = -sine;
		xform.eM22 = cosine;
		xform.eDx = (center_x - cosine * center_x + sine * center_y);
		xform.eDy = (center_y - cosine * center_y - sine * center_x);

		Render::transform_set(surface, xform);
		center_x -= xoffset * xscale;
		center_y -= yoffset * yscale;
		image.Draw(surface, center_x, center_y, width * abs(xscale), height * abs(yscale), 0, 0, width, height);

		//Render::transform_set_identity(surface);
		xform.eM11 = (float)1.0;
		xform.eM12 = (float)0;
		xform.eM21 = (float)0;
		xform.eM22 = (float)1.0;
		xform.eDx = (float)0;
		xform.eDy = (float)0;

		SetWorldTransform(surface, &xform);
		SetGraphicsMode(surface, nGraphicsMode);
	} else {
		double tx = dx - xoffset * xscale;
		double ty = dy - yoffset * yscale;

		image.Draw(surface, tx, ty, width * abs(xscale), height * abs(yscale), 0, 0, width, height);
	}
}

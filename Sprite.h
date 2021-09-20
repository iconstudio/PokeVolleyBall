#pragma once


class GameSprite {
public:
	GameSprite(HINSTANCE, const UINT, const UINT = 1, const int = 0, const int = 0);
	GameSprite(const LPCTSTR, const UINT = 1, const int = 0, const int = 0);
	~GameSprite();

	void draw(HDC, const double, const double, const UINT = 0, const double = 0.0, const double = 1.0, const double = 1.0);

	void set_bbox(const LONG left, const LONG right, const LONG top, const LONG bottom);

	const int get_width() const;
	const int get_height() const;

	const UINT number; // 이미지 장수
	const int xoffset, yoffset; // 스프라이트의 중심점
	RECT bbox; 

private:
	bool __process_image(CImage&, const size_t = 0, const size_t = 0);
	void __draw_single(HDC, CImage&, const double, const double, const double = 0.0, const double = 1.0, const double = 1.0);

	CImage raw; // 원본 그림.
	size_t raw_width, raw_height; // 원본 크기

	vector<unique_ptr<CImage>> frames; // 잘린 그림. 비어있을 수도 있다.
	size_t width, height; // 프레임 크기
};

/*
 * This file is part of EasyRPG Player.
 *
 * EasyRPG Player is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EasyRPG Player is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
 */

// Headers
#include <algorithm>
#include <sstream>
#include <vector>
#include <list>

#include "graphics.h"
#include "bitmap.h"
#include "cache.h"
#include "baseui.h"
#include "drawable.h"
#include "util_macro.h"
#include "output.h"
#include "player.h"
#ifdef EASYRPG_SYMBIAN
#include "RenderDamage.h"
#endif

namespace Graphics {
	void UpdateTitle();
	void DrawFrame();
	void DrawFrameContents();
	void DrawOverlay();

	int fps;
	int framerate;

	void UpdateTransition();

	BitmapRef frozen_screen;
	BitmapRef black_screen;
	BitmapRef transition_buffer;
	BitmapRef screen1;
	BitmapRef screen2;
	bool frozen;
	TransitionType transition_type;
	int transition_duration;
	int transition_frames_left;
	int transition_frame;
	bool screen_erased;

	uint32_t next_fps_time;

	struct State {
		State() {}
		std::list<Drawable*> drawable_list;
		bool zlist_dirty = false;
		bool draw_background = true;
	};

	int real_fps;

	std::shared_ptr<State> state;
	std::vector<std::shared_ptr<State> > stack;
	std::shared_ptr<State> global_state;

	bool SortDrawableList(const Drawable* first, const Drawable* second);
}

unsigned SecondToFrame(float const second) {
	return(second * Graphics::framerate);
}

void Graphics::Init() {
	fps = 0;
	frozen_screen = BitmapRef();
	screen_erased = false;
	transition_frames_left = 0;

	black_screen = Bitmap::Create(DisplayUi->GetWidth(), DisplayUi->GetHeight(), Color(0,0,0,255));
	transition_buffer = Bitmap::Create(DisplayUi->GetWidth() / 2 + 1, DisplayUi->GetHeight() / 2 + 1, false);

	state.reset(new State());
	global_state.reset(new State());

	next_fps_time = 0;
}

void Graphics::Quit() {
	state->drawable_list.clear();
	global_state->drawable_list.clear();

	frozen_screen.reset();
	black_screen.reset();
	transition_buffer.reset();

	Cache::Clear();
}

void Graphics::Update(bool time_left) {
	if (next_fps_time == 0) {
		next_fps_time = DisplayUi->GetTicks() + 1000;
	}

	// Calculate fps
	uint32_t current_time = DisplayUi->GetTicks();
	if (current_time >= next_fps_time) {
		// 1 sec over
		next_fps_time += 1000;
		real_fps = fps;

		if (fps == 0) {
			Output::Debug("Framerate is 0 FPS!");
#ifndef EASYRPG_SYMBIAN
			DrawFrame();
#endif
		}

		fps = 0;

		next_fps_time = current_time + 1000;

		UpdateTitle();
	}

	// Render next frame
	if (time_left) {
		fps++;

		DrawFrame();
	}
#ifdef EASYRPG_SYMBIAN
	else if (transition_frames_left > 0) {
		transition_frame++;
		transition_frames_left--;
	}
#endif
}

void Graphics::UpdateTitle() {
	if (DisplayUi->IsFullscreen()) return;
#ifdef EMSCRIPTEN
	return;
#endif

	std::stringstream title;
	if (!Player::game_title.empty()) {
		title << Player::game_title << " - ";
	}
	title << GAME_TITLE;

	if (Player::fps_flag) {
		title << " - FPS " << real_fps;
	}

	DisplayUi->SetTitle(title.str());
}

void Graphics::DrawFrame() {
	if (transition_frames_left > 0) {
#ifdef EASYRPG_SYMBIAN
		SymbianRenderDamage::Reset();
		SymbianRenderDamage::ForceFullFrame(DisplayUi->GetWidth(), DisplayUi->GetHeight());
#endif
		UpdateTransition();

		for (Drawable* drawable : global_state->drawable_list) {
			drawable->Draw();
		}

		DrawOverlay();

		DisplayUi->UpdateDisplay();
		return;
	}

	if (screen_erased) {
#ifdef EASYRPG_SYMBIAN
		SymbianRenderDamage::Reset();
		SymbianRenderDamage::ForceFullFrame(DisplayUi->GetWidth(), DisplayUi->GetHeight());
#endif
		DisplayUi->CleanDisplay();
		return;
	}

#ifdef EASYRPG_SYMBIAN
	BitmapRef surface = DisplayUi->GetDisplaySurface();
	if (!surface) {
		return;
	}

	int pending_scroll_x = 0;
	int pending_scroll_y = 0;
	const bool moving_scene = SymbianRenderDamage::PendingScroll(pending_scroll_x, pending_scroll_y);
	if (SymbianRenderDamage::FullFrameRequested() || moving_scene) {
		surface->ClearClipRects();
		SymbianRenderDamage::BeginFullPaintRecord(surface->GetWidth(), surface->GetHeight());
		if (!state->draw_background)
			surface->Fill(Color(0, 0, 0, 255));
		DrawFrameContents();
		SymbianRenderDamage::EndFullPaintRecord();
		SymbianRenderDamage::DiscardScroll();
	} else {
		SymbianRenderDamage::BeginRecord(surface->GetWidth(), surface->GetHeight());
		DrawFrameContents();
		SymbianRenderDamage::EndRecord();

		if (SymbianRenderDamage::NeedsPaint()) {
			const int dirtyCount = SymbianRenderDamage::DirtyCount();
			if (SymbianRenderDamage::IsFullFrameDirty()) {
				surface->ClearClipRects();
				SymbianRenderDamage::BeginPaint(0);
				if (!state->draw_background)
					surface->Fill(Color(0, 0, 0, 255));
				DrawFrameContents();
				SymbianRenderDamage::EndPaint();
			} else {
				for (int index = 0; index < dirtyCount; ++index) {
					const Rect dirty = SymbianRenderDamage::DirtyRect(index);
					surface->SetClipRects(&dirty, 1);
					SymbianRenderDamage::BeginPaint(index);
					if (!state->draw_background)
						surface->FillRect(dirty, Color(0, 0, 0, 255));
					DrawFrameContents();
					SymbianRenderDamage::EndPaint();
				}
				surface->ClearClipRects();
			}
		}
	}
#else
	DrawFrameContents();
#endif

	DisplayUi->UpdateDisplay();
}

void Graphics::DrawFrameContents() {
	if (state->zlist_dirty) {
		state->drawable_list.sort(SortDrawableList);
		state->zlist_dirty = false;
	}

	if (global_state->zlist_dirty) {
		global_state->drawable_list.sort(SortDrawableList);
		global_state->zlist_dirty = false;
	}

	if (state->draw_background) {
		DisplayUi->AddBackground();
	}

	for (Drawable* drawable : state->drawable_list) {
		drawable->Draw();
	}

	for (Drawable* drawable : global_state->drawable_list) {
		drawable->Draw();
	}

	DrawOverlay();
}

void Graphics::DrawOverlay() {
	if (
#ifndef EMSCRIPTEN
		DisplayUi->IsFullscreen() &&
#endif
		Player::fps_flag) {
		std::stringstream text;
		text << "FPS: " << real_fps;
		DisplayUi->GetDisplaySurface()->TextDraw(2, 2, Color(255, 255, 255, 255), text.str());
	}
}

BitmapRef Graphics::SnapToBitmap() {
	if (state->draw_background) {
		DisplayUi->AddBackground();
	}

	for (Drawable* drawable : state->drawable_list) {
		drawable->Draw();
	}

	for (Drawable* drawable : global_state->drawable_list) {
		drawable->Draw();
	}

	return DisplayUi->CaptureScreen();
}

void Graphics::Freeze() {
	frozen_screen = SnapToBitmap();
}

void Graphics::Transition(TransitionType type, int duration, bool erase) {
	if (screen_erased && erase) {
		// Don't allow another erase when already erased
		return;
	}

	if (type != TransitionNone) {
		transition_type = type;
		transition_frame = 0;
		transition_duration = type == TransitionErase ? 1 : duration;
		transition_frames_left = transition_duration;

		if (state->zlist_dirty) {
			state->drawable_list.sort(SortDrawableList);
			state->zlist_dirty = false;
		}

		if (global_state->zlist_dirty) {
			global_state->drawable_list.sort(SortDrawableList);
			global_state->zlist_dirty = false;
		}

		Freeze();

		if (erase) {
			screen1 = frozen_screen;

			screen2 = black_screen;
		} else {
			screen2 = frozen_screen;

			if (screen_erased)
				screen1 = black_screen;
			else
				screen1 = screen2;
		}
	}

	screen_erased = erase;
}

bool Graphics::IsTransitionPending() {
	return transition_frames_left > 0;
}

void Graphics::UpdateTransition() {
	// FIXME: Comments. Pleeeease. screen1, screen2?
	BitmapRef dst = DisplayUi->GetDisplaySurface();
	int w = DisplayUi->GetWidth();
	int h = DisplayUi->GetHeight();

	transition_frame++;

	int percentage = transition_frame * 100 / transition_duration;

	transition_frames_left--;

	switch (transition_type) {
	case TransitionFadeIn:
	case TransitionFadeOut:
		dst->Blit(0, 0, *screen1, screen1->GetRect(), 255);
		dst->Blit(0, 0, *screen2, screen2->GetRect(), 255 * percentage / 100);
		break;
	case TransitionRandomBlocks:
	case TransitionRandomBlocksUp:
	case TransitionRandomBlocksDown: {
		const int block = 16;
		dst->Blit(0, 0, *screen1, screen1->GetRect(), 255);
		for (int y = 0; y < h; y += block) {
			for (int x = 0; x < w; x += block) {
				const unsigned int bx = static_cast<unsigned int>(x / block);
				const unsigned int by = static_cast<unsigned int>(y / block);
				unsigned int rank = (bx * 1103515245u + by * 12345u + (bx ^ (by << 4)) * 2654435761u) % 10000u;
				if (transition_type == TransitionRandomBlocksUp) {
					const unsigned int rows = static_cast<unsigned int>((h + block - 1) / block);
					rank = rows > 1 ? (by * 8000u / (rows - 1u) + rank / 5u) : rank;
				} else if (transition_type == TransitionRandomBlocksDown) {
					const unsigned int rows = static_cast<unsigned int>((h + block - 1) / block);
					rank = rows > 1 ? ((rows - 1u - by) * 8000u / (rows - 1u) + rank / 5u) : rank;
				}
				if (rank >= static_cast<unsigned int>(percentage * 100))
					continue;
				const int bw = std::min(block, w - x);
				const int bh = std::min(block, h - y);
				dst->Blit(x, y, *screen2, Rect(x, y, bw, bh), 255);
			}
		}
		break;
	}
	case TransitionBlindOpen:
		for (int i = 0; i < h / 8; i++) {
			dst->Blit(0, i * 8, *screen1, Rect(0, i * 8, w, 8 - 8 * percentage / 100), 255);
			dst->Blit(0, i * 8 + 8 - 8 * percentage / 100, *screen2, Rect(0, i * 8 + 8 - 8 * percentage / 100, w, 8 * percentage / 100), 255 * percentage / 100);
		}
		break;
	case TransitionBlindClose:
		for (int i = 0; i < h / 8; i++) {
			dst->Blit(0, i * 8 + 8 * percentage / 100, *screen1, Rect(0, i * 8 + 8 * percentage / 100, w, 8 - 8 * percentage / 100), 255);
			dst->Blit(0, i * 8, *screen2, Rect(0, i * 8, w, 8 * percentage / 100), 255);
		}
		break;
	case TransitionVerticalStripesIn:
	case TransitionVerticalStripesOut:
		for (int i = 0; i < h / 6 + 1 - h / 6 * percentage / 100; i++) {
			dst->Blit(0, i * 6 + 3, *screen1, Rect(0, i * 6 + 3, w, 3), 255);
			dst->Blit(0, h - i * 6, *screen1, Rect(0, h - i * 6, w, 3), 255);
		}
		for (int i = 0; i < h / 6 * percentage / 100; i++) {
			dst->Blit(0, i * 6, *screen2, Rect(0, i * 6, w, 3), 255);
			dst->Blit(0, h - 3 - i * 6, *screen2, Rect(0, h - 3 - i * 6, w, 3), 255);
		}
		break;
	case TransitionHorizontalStripesIn:
	case TransitionHorizontalStripesOut:
		for (int i = 0; i < w / 8 + 1 - w / 8 * percentage / 100; i++) {
			dst->Blit(i * 8 + 4, 0, *screen1, Rect(i * 8 + 4, 0, 4, h), 255);
			dst->Blit(w - i * 8, 0, *screen1, Rect(w - i * 8, 0, 4, h), 255);
		}
		for (int i = 0; i < w / 8 * percentage / 100; i++) {
			dst->Blit(i * 8, 0, *screen2, Rect(i * 8, 0, 4, h), 255);
			dst->Blit(w - 4 - i * 8, 0, *screen2, Rect(w - 4 - i * 8, 0, 4, h), 255);
		}
		break;
	case TransitionBorderToCenterIn:
	case TransitionBorderToCenterOut:
		dst->Blit(0,  0, *screen2, screen2->GetRect(), 255);
		dst->Blit((w / 2) * percentage / 100, (h / 2) * percentage / 100, *screen1, Rect((w / 2) * percentage / 100, (h / 2) * percentage / 100, w - w * percentage / 100, h - h * percentage / 100), 255);
		break;
	case TransitionCenterToBorderIn:
	case TransitionCenterToBorderOut:
		dst->Blit(0,  0, *screen1, screen1->GetRect(), 255);
		dst->Blit(w / 2 - (w / 2) * percentage / 100, h / 2 - (h / 2) * percentage / 100, *screen2, Rect(w / 2 - (w / 2) * percentage / 100, h / 2 - (h / 2) * percentage / 100, w * percentage / 100, h * percentage / 100), 255);
		break;
	case TransitionScrollUpIn:
	case TransitionScrollUpOut:
		dst->Blit(0,  -h * percentage / 100, *screen1, screen1->GetRect(), 255);
		dst->Blit(0,  h - h * percentage / 100, *screen2, screen2->GetRect(), 255);
		break;
	case TransitionScrollDownIn:
	case TransitionScrollDownOut:
		dst->Blit(0,  h * percentage / 100, *screen1, screen1->GetRect(), 255);
		dst->Blit(0,  -h + h * percentage / 100, *screen2, screen2->GetRect(), 255);
		break;
	case TransitionScrollLeftIn:
	case TransitionScrollLeftOut:
		dst->Blit(-w * percentage / 100,  0, *screen1, screen1->GetRect(), 255);
		dst->Blit(w - w * percentage / 100,  0, *screen2, screen2->GetRect(), 255);
		break;
	case TransitionScrollRightIn:
	case TransitionScrollRightOut:
		dst->Blit(w * percentage / 100,  0, *screen1, screen1->GetRect(), 255);
		dst->Blit(-w + w * percentage / 100,  0, *screen2, screen2->GetRect(), 255);
		break;
	case TransitionVerticalCombine:
		dst->Blit(0, (h / 2) * percentage / 100, *screen1, Rect(0, (h / 2) * percentage / 100, w, h - h * percentage / 100), 255);
		dst->Blit(0, -h / 2 + (h / 2) * percentage / 100, *screen2, Rect(0, 0, w, h / 2), 255);
		dst->Blit(0, h - (h / 2) * percentage / 100, *screen2, Rect(0, h / 2, w, h / 2), 255);
		break;
	case TransitionVerticalDivision:
		dst->Blit(0, -(h / 2) * percentage / 100, *screen1, Rect(0, 0, w, h / 2), 255);
		dst->Blit(0, h / 2 + (h / 2) * percentage / 100, *screen1, Rect(0, h / 2, w, h / 2), 255);
		dst->Blit(0, h / 2 - (h / 2) * percentage / 100, *screen2, Rect(0, h / 2 - (h / 2) * percentage / 100, w, h * percentage / 100), 255);
		break;
	case TransitionHorizontalCombine:
		dst->Blit((w / 2) * percentage / 100, 0, *screen1, Rect((w / 2) * percentage / 100, 0, w - w * percentage / 100, h), 255);
		dst->Blit(- w / 2 + (w / 2) * percentage / 100, 0, *screen2, Rect(0, 0, w / 2, h), 255);
		dst->Blit(w - (w / 2) * percentage / 100, 0, *screen2, Rect(w / 2, 0, w / 2, h), 255);
		break;
	case TransitionHorizontalDivision:
		dst->Blit(-(w / 2) * percentage / 100, 0, *screen1, Rect(0, 0, w / 2, h), 255);
		dst->Blit(w / 2 + (w / 2) * percentage / 100, 0, *screen1, Rect(w / 2, 0, w / 2, h), 255);
		dst->Blit(w / 2 - (w / 2) * percentage / 100, 0, *screen2, Rect(w / 2 - (w / 2) * percentage / 100, 0, w * percentage / 100, h), 255);
		break;
	case TransitionCrossCombine:
		dst->Blit((w / 2) * percentage / 100, 0, *screen1, Rect((w / 2) * percentage / 100, 0, w - w * percentage / 100, (h / 2) * percentage / 100), 255);
		dst->Blit((w / 2) * percentage / 100, h - (h / 2) * percentage / 100, *screen1, Rect((w / 2) * percentage / 100, h - (h / 2) * percentage / 100, w - w * percentage / 100, (h / 2) * percentage / 100), 255);
		dst->Blit(0, (h / 2) * percentage / 100, *screen1, Rect(0, (h / 2) * percentage / 100, w, h - h * percentage / 100), 255);
		dst->Blit(- w / 2 + (w / 2) * percentage / 100, -h / 2 + (h / 2) * percentage / 100, *screen2, Rect(0, 0, w / 2, h / 2), 255);
		dst->Blit(w - (w / 2) * percentage / 100, -h / 2 + (h / 2) * percentage / 100, *screen2, Rect(w / 2, 0, w / 2, h / 2), 255);
		dst->Blit(w - (w / 2) * percentage / 100, h - (h / 2) * percentage / 100, *screen2, Rect(w / 2, h / 2, w / 2, h / 2), 255);
		dst->Blit(- w / 2 + (w / 2) * percentage / 100, h - (h / 2) * percentage / 100, *screen2, Rect(0, h / 2, w / 2, h / 2), 255);
		break;
	case TransitionCrossDivision:
		dst->Blit(-(w / 2) * percentage / 100, -(h / 2) * percentage / 100, *screen1, Rect(0, 0, w / 2, h / 2), 255);
		dst->Blit(w / 2 + (w / 2) * percentage / 100, -(h / 2) * percentage / 100, *screen1, Rect(w / 2, 0, w / 2, h / 2), 255);
		dst->Blit(w / 2 + (w / 2) * percentage / 100, h / 2 + (h / 2) * percentage / 100, *screen1, Rect(w / 2, h / 2, w / 2, h / 2), 255);
		dst->Blit(-(w / 2) * percentage / 100, h / 2 + (h / 2) * percentage / 100, *screen1, Rect(0, h / 2, w / 2, h / 2), 255);
		dst->Blit(w / 2 - (w / 2) * percentage / 100, 0, *screen2, Rect(w / 2 - (w / 2) * percentage / 100, 0, w * percentage / 100, h / 2 - (h / 2) * percentage / 100), 255);
		dst->Blit(w / 2 - (w / 2) * percentage / 100, h / 2 + (h / 2) * percentage / 100, *screen2, Rect(w / 2 - (w / 2) * percentage / 100, h / 2 + (h / 2) * percentage / 100, w * percentage / 100, h / 2 + (h / 2) * percentage / 100), 255);
		dst->Blit(0, h / 2 - (h / 2) * percentage / 100, *screen2, Rect(0, h / 2 - (h / 2) * percentage / 100, w, h * percentage / 100), 255);
		break;
	case TransitionZoomIn: {
		dst->Blit(0, 0, *screen1, screen1->GetRect(), 255);
		const int zw = std::max(1, w * percentage / 100);
		const int zh = std::max(1, h * percentage / 100);
		dst->StretchBlit(Rect((w - zw) / 2, (h - zh) / 2, zw, zh), *screen2, screen2->GetRect(), 255);
		break;
	}
	case TransitionZoomOut: {
		dst->Blit(0, 0, *screen2, screen2->GetRect(), 255);
		const int remain = 100 - percentage;
		if (remain > 0) {
			const int zw = std::max(1, w * remain / 100);
			const int zh = std::max(1, h * remain / 100);
			dst->StretchBlit(Rect((w - zw) / 2, (h - zh) / 2, zw, zh), *screen1, screen1->GetRect(), 255);
		}
		break;
	}
	case TransitionMosaicIn:
	case TransitionMosaicOut: {
		const int progress = transition_type == TransitionMosaicIn ? percentage : 100 - percentage;
		int block = 1 + (100 - progress) * 15 / 100;
		if (block <= 1 || !transition_buffer) {
			dst->Blit(0, 0, transition_type == TransitionMosaicIn ? *screen2 : *screen1,
				transition_type == TransitionMosaicIn ? screen2->GetRect() : screen1->GetRect(), 255);
		} else {
			const BitmapRef& source = transition_type == TransitionMosaicIn ? screen2 : screen1;
			const int sw = (w + block - 1) / block;
			const int sh = (h + block - 1) / block;
			transition_buffer->StretchBlit(Rect(0, 0, sw, sh), *source, source->GetRect(), 255);
			dst->StretchBlit(Rect(0, 0, w, h), *transition_buffer, Rect(0, 0, sw, sh), 255);
		}
		if (transition_type == TransitionMosaicOut)
			dst->Blit(0, 0, *screen2, screen2->GetRect(), 255 * percentage / 100);
		break;
	}
	case TransitionWaveIn: {
		dst->Blit(0, 0, *screen1, screen1->GetRect(), 255);
		const int depth = (100 - percentage) * 16 / 100;
		dst->WaverBlit(0, 0, 1.0, 1.0, *screen2, screen2->GetRect(), depth,
			static_cast<double>(transition_frame) * 0.25, 255 * percentage / 100);
		break;
	}
	case TransitionWaveOut: {
		dst->Blit(0, 0, *screen2, screen2->GetRect(), 255);
		const int depth = percentage * 16 / 100;
		dst->WaverBlit(0, 0, 1.0, 1.0, *screen1, screen1->GetRect(), depth,
			static_cast<double>(transition_frame) * 0.25, 255 * (100 - percentage) / 100);
		break;
	}
	default:
		DisplayUi->CleanDisplay();
		break;
	}
}

void Graphics::FrameReset() {
	next_fps_time = (uint32_t)DisplayUi->GetTicks() + 1000;
	fps = 0;
}

void Graphics::RegisterDrawable(Drawable* drawable) {
	if (drawable->IsGlobal()) {
		global_state->drawable_list.push_back(drawable);
		global_state->zlist_dirty = true;
	} else {
		state->drawable_list.push_back(drawable);
		state->zlist_dirty = true;
	}
}

void Graphics::RemoveDrawable(Drawable* drawable) {
	std::list<Drawable*>::iterator it;
	if (drawable->IsGlobal()) {
		it = std::find(global_state->drawable_list.begin(), global_state->drawable_list.end(), drawable);
		if (it != global_state->drawable_list.end()) { global_state->drawable_list.erase(it); }
	} else {
		it = std::find(state->drawable_list.begin(), state->drawable_list.end(), drawable);
		if (it != state->drawable_list.end()) { state->drawable_list.erase(it); }
	}
}

void Graphics::UpdateZCallback() {
	state->zlist_dirty = true;
	global_state->zlist_dirty = true;
}

void Graphics::UpdateZCallback(Drawable* drawable) {
	if (drawable && drawable->IsGlobal())
		global_state->zlist_dirty = true;
	else
		state->zlist_dirty = true;
}

inline bool Graphics::SortDrawableList(const Drawable* first, const Drawable* second) {
	if (first->GetZ() < second->GetZ()) return true;
	return false;
}

void Graphics::Push(bool draw_background) {
	stack.push_back(state);
	state.reset(new State());
	state->draw_background = draw_background;
}

void Graphics::Pop() {
	if (stack.size() > 0) {
		state = stack.back();
		stack.pop_back();
	}
}

int Graphics::GetDefaultFps() {
	return DEFAULT_FPS;
}

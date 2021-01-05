/*
 * LEAF Visual Novel System For X
 * (c) Copyright 1999,2000 Go Watanabe mailto:go@denpa.org
 * All rights reserverd.
 *
 * ORIGINAL LVNS (c) Copyright 1996-1999 LEAF/AQUAPLUS Inc.
 *
 * $Id: sizuku_menu.c,v 1.10 2001/08/05 10:58:10 go Exp $
 *
 */

/*
 * 雫 しおり選択メニュー表示
 */

#include <stdio.h>
#include <string.h>
#include "sizuku.h"

/* ------------------------------------------------------------ */
/** ロード確認用メニュー */

static MenuLine load_confirm_menu_line[] = {
    MENULINE(4, "ロードします。", 0),
    MENULINE(5, "よろしいですか？", 0),
    MENULINE(7, "はい",1),
    MENULINE(8, "いいえ", 2),
	MENULINE(0, NULL, 0),
};

static MenuData load_confirm_menu = {
	load_confirm_menu_line,
	2, 0, 
	NULL,
};

/* ------------------------------------------------------------ */
/** セーブ確認用メニュー */

static MenuLine save_confirm_menu_line[] = {
	MENULINE(4, "セーブします。", 0),
    MENULINE(5, "よろしいですか？", 0),
    MENULINE(7, "はい",1),
    MENULINE(8, "いいえ", 2),
	MENULINE(0, NULL, 0),
};

static MenuData save_confirm_menu = {
	save_confirm_menu_line,
	2, 0, 
	NULL,
};

/* ------------------------------------------------------------ */
/** 終了確認メニュー */

static MenuLine end_confirm_menu_line[] = {
    MENULINE(4, "終了します。", 0),
    MENULINE(5, "よろしいですか？", 0),
    MENULINE(7, "はい", 1),
    MENULINE(8, "いいえ", 2),
	MENULINE(0, NULL, 0),
};

static MenuData end_confirm_menu = {
	end_confirm_menu_line,
	2, 0, 
	NULL,
};

/* 
 * 雫メニュー処理
 */
static MenuLine sizuku_menu_line[] = {
    MENULINE(3, "文字を消す",1),
	MENULINE(4, "ロードする", 2),
    MENULINE(5, "セーブする", 3),
    MENULINE(6, "シナリオ回想", 4),
    MENULINE(7, "一つ前の選択肢に戻る", 5),
    MENULINE(8, "ゲーム終了", 6),
	MENULINE(0, NULL, 0),
};

static int
SizukuMenuEngine(Lvns *lvns, int select)
{
    SizukuState *state = (SizukuState *)lvns->system_state;
    switch (select) {

    case 1:
		/* 画像表示モード */
		LvnsImageMode(lvns);
        break;

    case 2:
		/* ロード */
		switch (LvnsMenu(lvns, &load_confirm_menu, True)) {
		case -1: /* cancel */
			return 0;
		case 1:
			/* ロード処理 */
			SizukuLoad(lvns);
			LvnsClear(lvns, LVNS_EFFECT_WIPE_TTOB);
			LvnsJump(lvns, LVNS_JUMP_RESTART);
		case 2:			
			break;
		}
		break;

    case 3:
        /* セーブ */
		switch (LvnsMenu(lvns, &save_confirm_menu, True)) {
		case -1: /* cancel */
			return 0;
		case 1:
			/* セーブ処理 */
			SizukuSave(lvns);
		case 2:			
			break;
		}
		break;

	case 4:
		/* 回想モード */
		LvnsHistoryMode(lvns);
		break;

    case 5:
        /* 選択肢に戻る */
        lvns->savepoint = lvns->selectpoint;
        memcpy(state->flag_save, state->flag_select, sizeof state->flag);
		LvnsClear(lvns, LVNS_EFFECT_WIPE_TTOB);
		LvnsJump(lvns, LVNS_JUMP_RESTART);

    case 6:
        /* タイトルに戻る */
		switch (LvnsMenu(lvns, &end_confirm_menu, True)) {
		case -1: /* cancel */
			return 0;
		case 1:
			LvnsClear(lvns, LVNS_EFFECT_WIPE_TTOB);
			LvnsJump(lvns, LVNS_JUMP_TITLE);
		case 2:			
			break;
		}
		break;

    default:;
    }
    return select;
}

MenuData sizuku_menu = {
	sizuku_menu_line,
	6, 0, 
	SizukuMenuEngine
};

import os

from pygmy import *
# module pygmy:
#   def GetFreeBytes(path)   returns long
#   def GetDriveLetter(path) returns unicode
#   def GetTotalCount()      returns int
#   def GetTotalBytes()      returns long
#   def GetSelectedCount()   returns int
#   def GetSelectedBytes()   returns long
#   def Execute(file, args)
#   def MessageBox(message)
#   const int CONTROL, SHIFT, ALT, RIGHT
#   const int CANCEL, GOTO, GOTO_ALWAYS, OPEN, OPEN_ALWAYS, APPEND, RESERVE, SWITCH, REPLACE

#================================================================================
# Private Settings

def PathIsText(path):
    textfiles = ["README", "CHANGELOG", "MAKEFILE"]
    return os.path.basename(path).upper() in textfiles

#================================================================================
# Caption() キャプションの文字列を問い合わせられる。
# name : 現在のフォルダの名前。一つも無い場合はNone。
# path : 現在のフォルダのパス。一つも無いまたは仮想フォルダの場合はNone。
def Caption(name, path):
    basename = 'Avesta'
    separator = ' - '
    if path:
        return basename + separator + name + ' / ' + path
    elif name:
        return basename + separator + name
    else:
        return basename

#================================================================================

def FormatBytes(bytes, radix=1024):
    num = bytes
    unitList = {1000: ['B', 'KB', 'MB', 'GB', 'TB'],
                1024: ['B', 'KiB', 'MiB', 'GiB', 'TiB']}[radix]
    unit = unitList[-1]
    for u in unitList[:-1]:
        if num < radix:
            unit = u
            break;
        num /= radix
    return f'{num:.3f} {unit}'

# StatusText() ステータスバーの文字列を問い合わせられる。
# text : Windowsから返されるデフォルトのテキスト(unicode)
# name : 現在のフォルダの名前(unicode)
# path : 現在のフォルダのパス(unicode)
def StatusText(text, name, path):
    if text or not name:
        return text
    selectedCount = GetSelectedCount()
    totalCount = GetTotalCount()
    if selectedCount == 0:
        if path:
            totalBytes = GetTotalBytes()
            freeBytes = GetFreeBytes(path)
            drive = GetDriveLetter(path)
            return f'{totalCount:d} 個のオブジェクト ( {FormatBytes(totalBytes)} ) / 空きディスク領域 [{drive}] {FormatBytes(freeBytes)}'
        else:
            return f'{totalCount:d} 個のオブジェクト'
    else:
        selectedBytes = GetSelectedBytes()
        return f'{selectedCount:d} / {totalCount:d} 個のオブジェクトを選択 ( {FormatBytes(selectedBytes)} )'

#================================================================================
# GestureText() ジャスチャ中にステータスバーに表示される文字列を問い合わせられる。
# gestures : ジェスチャシーケンス(list of int)
# command : コマンドの説明(unicode)
def GestureText(gestures, command):
    geschr = ['左', '右', '中', '４', '５', '△', '▽', '←', '→', '↑', '↓']
    gesstr = ''.join([geschr[i] for i in gestures])
    if command:
        return f'ジェスチャ ： [{gesstr}] ({command})'
    else:
        return f'ジェスチャ ： [{gesstr}]'

#================================================================================

NormalGoto = {                           \
    0                     : GOTO,        \
    CONTROL               : APPEND,      \
              SHIFT       : RESERVE,     \
    CONTROL | SHIFT       : REPLACE,     \
                      ALT : GOTO,        \
    CONTROL |         ALT : OPEN_ALWAYS, \
              SHIFT | ALT : SWITCH,      \
    CONTROL | SHIFT | ALT : GOTO,        \
}

LockedGoto = {                           \
    0                     : APPEND,      \
    CONTROL               : GOTO,        \
              SHIFT       : RESERVE,     \
    CONTROL | SHIFT       : APPEND,      \
                      ALT : APPEND,      \
    CONTROL |         ALT : APPEND,      \
              SHIFT | ALT : SWITCH,      \
    CONTROL | SHIFT | ALT : APPEND,      \
}

NormalOpen = {                           \
    0                     : OPEN,        \
    CONTROL               : APPEND,      \
              SHIFT       : RESERVE,     \
    CONTROL | SHIFT       : REPLACE,     \
                      ALT : OPEN,        \
    CONTROL |         ALT : GOTO,        \
              SHIFT | ALT : SWITCH,      \
    CONTROL | SHIFT | ALT : OPEN,        \
}

LockedOpen = {                           \
    0                     : APPEND,      \
    CONTROL               : APPEND,      \
              SHIFT       : RESERVE,     \
    CONTROL | SHIFT       : APPEND,      \
                      ALT : APPEND,      \
    CONTROL |         ALT : GOTO,        \
              SHIFT | ALT : SWITCH,      \
    CONTROL | SHIFT | ALT : APPEND,      \
}

# NavigateVerb() フォルダ移動時のアクションを返す。
# folder   : 移動元のパス。新しく開く場合はアクティブタブ。
# location : 移動先のパス。
# mods     : 修飾キーの状態。
# locked   : 移動元がロックされているか否か。
# verb     : デフォルトのアクション。
def NavigateVerb(folder, location, mods, locked, verb):
    if mods == RIGHT:
        return APPEND
    if verb == GOTO:
        if locked:
            return LockedGoto[mods & (CONTROL | SHIFT | ALT)]
        else:
            return NormalGoto[mods & (CONTROL | SHIFT | ALT)]
    elif verb == OPEN:
        if locked:
            return LockedOpen[mods & (CONTROL | SHIFT | ALT)]
        else:
            return NormalOpen[mods & (CONTROL | SHIFT | ALT)]
    else:
        return verb

#================================================================================
# ExecuteVerb() 項目実行時のアクションを返す。
# folder : 項目が置かれているフォルダのパス。
# item   : 実行しようとしている項目のパス。
# mods   : 修飾キーの状態。
def ExecuteVerb(folder, item, mods):
    if mods & CONTROL:
        return 'edit'
    elif mods & SHIFT:
        return '.txt'
    elif item:
        if PathIsText(item):
            return '.txt'
    return ''

#================================================================================
# WallPaper() 壁紙を返す。
# name : 現在のフォルダの名前。一つも無い場合はNone。
# path : 現在のフォルダのパス。一つも無いまたは仮想フォルダの場合はNone。
def WallPaper(filename, name, path):
    return filename

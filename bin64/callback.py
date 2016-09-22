# -*- coding: mbcs -*-

import string, os

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
# Caption() �L���v�V�����̕������₢���킹����B
# name : ���݂̃t�H���_�̖��O�B��������ꍇ��None�B
# path : ���݂̃t�H���_�̃p�X�B��������܂��͉��z�t�H���_�̏ꍇ��None�B
def Caption(name, path):
	basename = u"Avesta"
	separator = u" - "
	if path:
		return basename + separator + name + u" / " + path
	elif name:
		return basename + separator + name
	else:
		return basename

#================================================================================

def FormatBytes(bytes, radix = 1024):
	num = bytes
	unitList = {1000:["B", "KB", "MB", "GB", "TB"],
				1024:["B", "KiB", "MiB", "GiB", "TiB"]}[radix]
	unit = unitList[-1]
	for u in unitList[:-1]:
		if num < radix:
			unit = u
			break;
		num /= radix
	return "{0:.3f} {1}".format(num, unit)

# StatusText() �X�e�[�^�X�o�[�̕������₢���킹����B
# text : Windows����Ԃ����f�t�H���g�̃e�L�X�g(unicode)
# name : ���݂̃t�H���_�̖��O(unicode)
# path : ���݂̃t�H���_�̃p�X(unicode)
def StatusText(text, name, path):
	if text or not name:
		return text
	selectedCount = GetSelectedCount()
	totalCount = GetTotalCount()
	if selectedCount == 0:
		if path:
			totalBytes = GetTotalBytes()
			freeBytes  = GetFreeBytes(path)
			drive      = GetDriveLetter(path)
			return u"{0:d} �̃I�u�W�F�N�g ( {1} ) / �󂫃f�B�X�N�̈� [{2}] {3}".format(totalCount, FormatBytes(totalBytes), drive, FormatBytes(freeBytes))
		else:
			return u"{0:d} �̃I�u�W�F�N�g".format(totalCount)
	else:
		selectedBytes = GetSelectedBytes()
		return u"{0:d} / {1:d} �̃I�u�W�F�N�g��I�� ( {2} )".format(selectedCount, totalCount, FormatBytes(selectedBytes))

#================================================================================
# GestureText() �W���X�`�����ɃX�e�[�^�X�o�[�ɕ\������镶�����₢���킹����B
# gestures : �W�F�X�`���V�[�P���X(list of int)
# command : �R�}���h�̐���(unicode)
def GestureText(gestures, command):
	geschr = [u"��", u"�E", u"��", u"�S", u"�T", u"��", u"��", u"��", u"��", u"��", u"��"]
	gesstr = "".join([geschr[i] for i in gestures])
	if command:
		return u"�W�F�X�`�� �F [{0}] ({1})".format(gesstr, command)
	else:
		return u"�W�F�X�`�� �F [{0}]".format(gesstr)

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

# NavigateVerb() �t�H���_�ړ����̃A�N�V������Ԃ��B
# folder   : �ړ����̃p�X�B�V�����J���ꍇ�̓A�N�e�B�u�^�u�B
# location : �ړ���̃p�X�B
# mods     : �C���L�[�̏�ԁB
# locked   : �ړ��������b�N����Ă��邩�ۂ��B
# verb     : �f�t�H���g�̃A�N�V�����B
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
# ExecuteVerb() ���ڎ��s���̃A�N�V������Ԃ��B
# folder : ���ڂ��u����Ă���t�H���_�̃p�X�B
# item   : ���s���悤�Ƃ��Ă��鍀�ڂ̃p�X�B
# mods   : �C���L�[�̏�ԁB
def ExecuteVerb(folder, item, mods):
	if mods & CONTROL:
		return u"edit"
	elif mods & SHIFT:
		return u".txt"
	elif item:
		if PathIsText(item):
			return u".txt"
	return u""

#================================================================================
# WallPaper() �ǎ���Ԃ��B
# name : ���݂̃t�H���_�̖��O�B��������ꍇ��None�B
# path : ���݂̃t�H���_�̃p�X�B��������܂��͉��z�t�H���_�̏ꍇ��None�B
def WallPaper(filename, name, path):
	return filename

/// @file widgets.hpp
/// �E�B�W�F�b�g�K�w.
#pragma once

#include "application.hpp"
#include "signal.hpp"
#include "afx.hpp"

namespace mew
{
	namespace io
	{
		interface IEntry;
		__interface IEntryList;
	}

	/// �E�B�W�F�b�g�K�w.
	namespace ui
	{
		enum InternalMessage
		{
			MEW_WM_FIRST		= WM_USER + 1000,

			//======================================================================
			// �񓯊��R�}���h�n
			//======================================================================

			MEW_ECHO_UPDATE,	// (void)wParam, (void)lParam
			MEW_ECHO_COPYDATA,	// (void)wParam, (IString*)lParam

			//======================================================================
			// �ʒm�n
			//======================================================================

			MEW_NM_SHELL,		// SHChangeNotifyRegister
			MEW_NM_TASKTRAY,	// Shell_NotifyIcon

			//======================================================================
			// �N�G���n
			//======================================================================

			MEW_QUERY_INTERFACE,	/// HRESULT QueryInterface(const IID* wParam, void** lParam)
			MEW_QUERY_GESTURE,		/// bool (IGesture** wParam, POINTS lParam)
			MEW_QUERY_DROP,			/// bool (IDropTarget** wParam, POINTS lParam)
		};

		//==============================================================================
		// �C���^�t�F�[�X

		// basic windows
		interface __declspec(uuid("F9CC1F5C-8041-48D8-8F2E-C000FEAE0B46")) IWindow;
		interface __declspec(uuid("C399ACBE-FE54-4EBF-B39D-6974F4616E24")) IList;
		interface __declspec(uuid("F2989F89-2E1E-43A8-9543-619A86FCCBAD")) ITree;
		
		// basic controls
		interface __declspec(uuid("3D7134DE-EF5D-4965-921F-75C997D0D35D")) IListView;
		interface __declspec(uuid("2A10F046-6C29-4169-976B-8653954AD083")) ITreeView;
		interface __declspec(uuid("4FC56D6B-3250-4636-8C97-439CBA76A7C8")) ITabPanel;
		interface __declspec(uuid("56795899-5291-4AFF-AC38-FABB00A63AF9")) IForm;
		interface __declspec(uuid("AAFCFDA5-87FA-4478-A341-DD68E645A93E")) IDisplay;

		// io windows : mew::io �Ɉړ����邩���B
		interface __declspec(uuid("9278A704-5BE3-4B73-92CE-D66DB650E9C8")) IPreview;
		interface __declspec(uuid("1943A27C-EA08-4C48-9FAA-9E193E3168EE")) IShellListView;
		__interface __declspec(uuid("60CFE2DB-365B-4972-88C5-2D2BDB922D02")) IShellStorage;

		// extented attributes
		__interface __declspec(uuid("12B10280-CC9C-4BDB-B442-33750FCF3B01")) IWallPaper;

		// command and view-items
		__interface __declspec(uuid("738D6988-289E-4E5B-A144-B75F22996EE2")) ITreeItem;
		__interface __declspec(uuid("69DBFA8F-A8FC-4520-A256-068FC8058FE0")) IEditableTreeItem;

		// window extensions
		__interface __declspec(uuid("84FC60AA-E00E-42B3-97B3-CF02A4FBF7EF")) IKeymap;
		__interface __declspec(uuid("6C815DC3-1A62-451C-AA25-D1DC1620EBAA")) IKeymapTable;
		__interface __declspec(uuid("D980EBD5-AC2C-413B-A887-A7125DB0E68B")) IGesture;
		__interface __declspec(uuid("7FB98F0C-F020-4B31-AB46-8D108D7441A7")) IGestureTable;

		// dialogs
		__interface __declspec(uuid("209A447F-DBEE-4178-B770-B73AD1B9719C")) IWallPaperDialog;
		__interface __declspec(uuid("02B6F4BE-8EFD-4573-85AC-A3A7D2131A2A")) IExpose;

		//==============================================================================
		// �_�C�A���O�֐�.

		/// �o�[�W��������\������.
		void AboutDialog(HWND hwnd, string module = null);
		/// �ǂݍ��݃t�@�C������₢���킹��.
		string OpenDialog(HWND hwnd, string filter);
		/// �����o���t�@�C������₢���킹��.
		string SaveDialog(HWND hwnd, string filter, PCTSTR strDefExt = null);
		/// �t�H���g�I���_�C�A���O.
		void FontDialog(HWND hwnd, HFONT hFont, string caption, function apply);

		//==============================================================================
		// �쐬�\�ȃN���X

		class __declspec(uuid("5EDC9069-65BC-4095-8E2C-1D2BAC74CF8C")) Display;
		class __declspec(uuid("EBB3ACF9-1AFA-45CF-ABF0-552F465F0375")) Form;
		class __declspec(uuid("9CB0D88E-4FD8-4222-AF16-580C826B9272")) MenuBar;
		class __declspec(uuid("32A55C2D-0D1E-429D-B89E-E3678B1EA5F5")) ToolBar;		
		class __declspec(uuid("2C5168C6-1679-4EAB-A55B-05F40763A947")) LinkBar;		
		class __declspec(uuid("B5A66FD0-85EF-4732-B2D3-0DB684DC505D")) TreeView;	
		class __declspec(uuid("EE72AD26-62B6-42D0-B6B0-E242F30E4510")) DockPanel;	
		class __declspec(uuid("96BC8DCC-8824-47C1-98CF-9C6E7C87DBB3")) TabPanel;	
		class __declspec(uuid("4BD891C3-0AD0-436D-BF80-10225A076683")) ReBar;		
		class __declspec(uuid("210CCBAE-4429-42AF-A1B0-1C5E1514A616")) ShellListView;
		class __declspec(uuid("90973EDF-FBB4-4B1D-9406-FE7CC9E0A0BF")) ShellTreeView;
		class __declspec(uuid("236CBFD9-6E4D-4336-AE91-3E60EE8B54BC")) Preview;
		class __declspec(uuid("6C27F5A7-67B5-4418-9FB5-4C4A4D3D7376")) StatusBar;
		class __declspec(uuid("27DE63DE-35FC-4E1F-8F6B-405950ECE85E")) DefaultTreeItem;
		class __declspec(uuid("9B81653E-F275-47E9-9136-4928C49C9382")) KeymapTable;
		class __declspec(uuid("B1435AF3-E660-4CD5-9D63-80ADF6C507F4")) GestureTable;
		class __declspec(uuid("EE813675-A87E-467B-90CD-A17EA1543BF1")) WallPaperDialog;
		class __declspec(uuid("10106D22-9371-4F07-B57F-E96655CE5BF0")) Expose;		

		//==============================================================================
		// �萔

		/// �}�E�X�{�^���ƏC���L�[.
		enum MouseAndModifier
		{
			MouseButtonNone		= 0x0000, ///< �Ȃ�.
			MouseButtonLeft		= 0x0001, ///< ���{�^��.
			MouseButtonRight	= 0x0002, ///< �E�{�^��.
			MouseButtonMiddle	= 0x0010, ///< ���{�^��.
			MouseButtonX1		= 0x0020, ///< �ǉ��{�^��1.
			MouseButtonX2		= 0x0040, ///< �ǉ��{�^��2.
			MouseButtonMask		= 0x0073, ///< �}�E�X�̃{�^�����ׂ�.

			ModifierNone		= 0x0000, ///< �Ȃ�.
			ModifierShift		= 0x0004, ///< �V�t�g�L�[.
			ModifierControl		= 0x0008, ///< �R���g���[���L�[.
			ModifierAlt			= 0x0100, ///< Alt.
			ModifierWindows		= 0x0200, ///< Windows.
			ModifierMask		= 0x030C, ///< �C���L�[���ׂ�.
		};

		/// �����X�^�C��.
		enum Direction
		{
			DirNone			= 0x0000, ///< �����Ȃ�.
			DirCenter		= 0x0001, ///< ����.
			DirWest			= 0x0010, ///< ���E��.
			DirEast			= 0x0020, ///< ���E�E.
			DirNorth		= 0x0040, ///< �k�E��.
			DirSouth		= 0x0080, ///< ��E��.
			DirMaskWE		= DirWest  | DirEast, ///< �����E���E�}�X�N.
			DirMaskNS		= DirNorth | DirSouth, ///< ��k�E�㉺�}�X�N.
		};

		/// �}�E�X�W�F�X�`��.
		enum Gesture
		{
			GestureButtonLeft,
			GestureButtonRight,
			GestureButtonMiddle,
			GestureButtonX1,
			GestureButtonX2,
			GestureWheelUp,
			GestureWheelDown,
			GestureWest,
			GestureEast,
			GestureNorth,
			GestureSouth,
		};

		//==============================================================================
		// �C���^�t�F�[�X��`.

		/// �E�B���h�E.
		interface __declspec(novtable) IWindow : ISignal
		{
#ifndef DOXYGEN
			virtual HWND      get_Handle() = 0;
			virtual Size      get_DefaultSize() = 0;
			virtual Direction get_Dock() = 0;
			virtual void      set_Dock(Direction value) = 0;
//			virtual IKeymap*  get_Keymap() = 0;
//			virtual void      set_Keymap(IKeymap* value) = 0;
//			virtual IGesture* get_Gesture() = 0;
//			virtual void      set_Gesture(IGesture* value) = 0;

			string  get_Name()					{ return afx::GetName(get_Handle()); }
			void    set_Name(string value)		{ afx::SetName(get_Handle(), value); }
			bool    get_Visible()				{ return afx::GetVisible(get_Handle()); }
			void    set_Visible(bool value)		{ afx::SetVisible(get_Handle(), value); }
			Rect    get_Bounds()				{ return afx::GetBounds(get_Handle()); }
			void    set_Bounds(Rect value)		{ afx::SetBounds(get_Handle(), value); }
			Point   get_Location()				{ return afx::GetLocation(get_Handle()); }
			void    set_Location(Point value)	{ afx::SetLocation(get_Handle(), value); }
			Rect    get_ClientArea()			{ return afx::GetClientArea(get_Handle()); }
			Size    get_ClientSize()			{ return afx::GetClientSize(get_Handle()); }
			void    set_ClientSize(Size value)	{ afx::SetClientSize(get_Handle(), value); }
#endif // DOXYGEN

			/// �E�B���h�E�n���h�� [get].
			__declspec(property(get=get_Handle)) HWND Handle;
			/// �]�܂����T�C�Y [get]. ���ɖ]�܂����T�C�Y�������Ȃ��ꍇ�́ASize::Zero ��Ԃ�.
			__declspec(property(get=get_DefaultSize)) Size DefaultSize;
			/// �h�b�L���O�X�^�C�� [get, set].
			__declspec(property(get=get_Dock, put=set_Dock)) Direction Dock;
			/// ���O [get, set].
			__declspec(property(get=get_Name, put=set_Name)) string Name;
			/// ����� [get, set].
			__declspec(property(get=get_Visible, put=set_Visible)) bool Visible;
			/// �E�B���h�E�O�ڗ̈� [get, set].
			/// �e�����ꍇ�́A�e���W�n�ł̈ʒu.
			/// ���[�g�E�B���h�E�̏ꍇ�́A�X�N���[�����W�n�ł̈ʒu.
			__declspec(property(get=get_Bounds, put=set_Bounds)) Rect Bounds;
			/// �E�B���h�E����ʒu [get, set].
			__declspec(property(get=get_Location, put=set_Location)) Point Location;
			/// �N���C�A���g�̈� [get].
			__declspec(property(get=get_ClientArea)) Rect ClientArea;
			/// �N���C�A���g�̈�̃T�C�Y [get, set].
			__declspec(property(get=get_ClientSize, put=set_ClientSize)) Size ClientSize;

			/// ���b�Z�[�W�𓯊����M����.
			virtual HRESULT Send(message msg) = 0;
			/// �E�B���h�E�����.
			/// ���łɔj���ς݂̏ꍇ�͉������Ȃ�.
			virtual void Close(bool sync = false) = 0;
			/// �ĕ`��.
			virtual void Update(bool sync = false) = 0;
			/// �g���n���h�����擾����.
			virtual HRESULT GetExtension(REFGUID which, REFINTF what) = 0;
			/// �g���n���h����ݒ肷��.
			/// @param which �g���̎��. ���Ȃ��Ƃ��ȉ��̊g���͎������ׂ�.
			///              - __uuidof(IGesture)     : �}�E�X�W�F�X�`����������.
			///              - __uuidof(IKeymap)      : �L�[�}�b�v��������.
			///              - __uuidof(IDropTarget)  : �h���b�v������������.
			/// @param what �i���n���h���I�u�W�F�N�g.
			virtual HRESULT SetExtension(REFGUID which, IUnknown* what) = 0;

			/// �t�H�[�J�X��^����.
//			HRESULT Close(message msg = null)		{ return avesta::WindowClose(get_Handle()); }
			HRESULT Focus(message msg = null)		{ return avesta::WindowSetFocus(get_Handle()); }
//			HRESULT Show (message msg = null)		{ return avesta::Show(get_Handle(), (msg["value"] | -1)); }

//			HRESULT Restore(message msg = null)		{ return afx::Restore(get_Handle()); }
//			HRESULT Resize(message msg = null)		{ return afx::Resize(get_Handle()); }
//			HRESULT Move(message msg = null)		{ return afx::Move(get_Handle()); }
//			HRESULT Maximize(message msg = null)	{ return afx::Maximize(get_Handle()); }
//			HRESULT Minimize(message msg = null)	{ return afx::Minimize(get_Handle()); }
		};

		/// ���X�g�A�C�e���R���e�i.
		interface __declspec(novtable) IList : IWindow
		{
#ifndef DOXYGEN
			virtual size_t get_Count() = 0;
#endif // DOXYGEN

			/// ���ڐ����擾����.
			__declspec(property(get=get_Count)) size_t Count;///< ���ڐ�.
			///
			virtual HRESULT GetAt(REFINTF pp, size_t index) = 0;
			/// �w��̏�Ԃ����A�C�e�����A�w��̃C���^�t�F�[�X�ɍ��킹�Ď擾����.
			/// @param ppInterface �擾����C���^�t�F�[�X. IEnumUnknown �̏ꍇ�͕����A����ȊO�͒P���̃A�C�e�����擾����.
			/// @param status �擾����A�C�e���̏��.
			virtual HRESULT GetContents(REFINTF ppInterface, Status status) = 0;
			/// �A�C�e���̏�Ԃ��擾����.
			virtual HRESULT GetStatus(
				IndexOr<IUnknown> item,	///< �Ώۂ̃A�C�e���܂��̓C���f�b�N�X.
				DWORD*   status,		///< �A�C�e���̏��. null�̏ꍇ�͎擾���Ȃ�.
				size_t*  index = null	///< �A�C�e���̃C���f�b�N�X. null�̏ꍇ�͎擾���Ȃ�.
			) = 0;
			/// �A�C�e���̏�Ԃ�ݒ肷��.
			virtual HRESULT SetStatus(
				IndexOr<IUnknown> item,	///< �Ώۂ̃A�C�e���܂��̓C���f�b�N�X.
				Status   status,		///< �A�C�e���̏��.
				bool     unique = false	///< �w�肵���A�C�e���ȊO��status��Ԃ��������邩�ۂ�.
			) = 0;
		};

		/// �c���[�A�C�e���R���e�i.
		interface __declspec(novtable) ITree : IWindow
		{
#ifndef DOXYGEN
			virtual ITreeItem*  get_Root() = 0;
			virtual void        set_Root(ITreeItem* value) = 0;
			virtual IImageList* get_ImageList() = 0;
			virtual void        set_ImageList(IImageList* value) = 0;
#endif // DOXYGEN

			/// ���[�g�c���[�A�C�e�� [get, set].
			__declspec(property(get=get_Root, put=set_Root)) ITreeItem* Root;
			/// �C���[�W���X�g [get, set].
			__declspec(property(get=get_ImageList, put=set_ImageList)) IImageList* ImageList;
		};

		enum CopyMode
		{
			CopyNone,
			CopyName,
			CopyPath,
			CopyBase,
		};

		/// �t���[���E�B���h�E.
		interface __declspec(novtable) IForm : ITree
		{
#ifndef DOXYGEN
			virtual bool		get_TaskTray() = 0;
			virtual void		set_TaskTray(bool value) = 0;
			virtual CopyMode	get_AutoCopy() = 0;
			virtual void		set_AutoCopy(CopyMode value) = 0;
#endif // DOXYGEN

			/// �^�X�N�g���C [get, set].
			__declspec(property(get=get_TaskTray, put=set_TaskTray)) bool TaskTray;
			/// �R�s�[���[�h [get, set].
			__declspec(property(get=get_AutoCopy, put=set_AutoCopy)) CopyMode AutoCopy;
		};

		/// �f�B�X�v���C.
		interface __declspec(novtable) IDisplay : IWindow
		{
			using WNDPROCEX = LRESULT (__stdcall *)(void* self, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

			virtual UINT PopupMenu(HMENU hMenu, UINT tpm, int x, int y, HWND hOwner, const RECT* rcExclude) throw() = 0;
			virtual size_t GetMenuDepth() throw() = 0;
			virtual HWND GetMenu(int index = -1) throw() = 0;
			virtual void RegisterMessageHook(void* self, WNDPROCEX wndproc) = 0;
			virtual void UnregisterMessageHook(void* self, WNDPROCEX wndproc) = 0;
		};

		/// �}���ʒu.
		enum InsertTo
		{
			InsertHead,	///< ��ԍŏ��ɑ}��.
			InsertTail,	///< ��ԍŌ�ɑ}��.
			InsertPrev,	///< ���݂̈�O�ɑ}��.
			InsertNext,	///< ���݂̈��ɑ}��.
		};

		/// ���ו�.
		enum ArrangeType
		{
			ArrangeHorz,	///< ���ɕ��ׂ�.
			ArrangeVert,	///< �c�ɕ��ׂ�.
			ArrangeAuto,	///< ���������ɕ��ׂ�.
		};

		/// �^�u�R���e�i.
		interface __declspec(novtable) ITabPanel : IList
		{
#ifndef DOXYGEN
			virtual InsertTo	get_InsertPosition() = 0;
			virtual void		set_InsertPosition(InsertTo pos) = 0;
			virtual ArrangeType	get_Arrange() = 0;
			virtual void		set_Arrange(ArrangeType value) = 0;
#endif // DOXYGEN

			/// �A�C�e���������ǉ�����ꍇ�̑}���ʒu [get, set].
			__declspec(property(get=get_InsertPosition, put=set_InsertPosition)) InsertTo InsertPosition;
			///< ���ו�.
			__declspec(property(get=get_Arrange, put=set_Arrange)) ArrangeType Arrange;
			///
			virtual Rect GetTabRect(IndexOr<IUnknown> item) = 0;
			///
			virtual HRESULT MoveTab(IndexOr<IUnknown> from, int to) = 0;
			/// �^�u�̍ŏ����A�ő啝���w�肷��.
			virtual HRESULT SetMinMaxTabWidth(size_t min, size_t max) = 0;
		};

		/// ���X�g�r���[�̕\���X�^�C��.
		enum ListStyle
		{
			ListStyleIcon		= 0x01,
			ListStyleLargeIcon	= ListStyleIcon,
			ListStyleSmallIcon	= 0x02,
			ListStyleList		= 0x03,
			ListStyleDetails	= 0x04,
			ListStyleThumnail	= 0x05,
			ListStyleTile		= 0x06,
		};

		/// ���X�g�r���[.
		interface __declspec(novtable) IListView : IList
		{
#ifndef DOXYGEN
			virtual size_t		get_SelectedCount() = 0;
			virtual ListStyle	get_Style() = 0;
			virtual void		set_Style(ListStyle value) = 0; 
			virtual bool		get_AutoArrange() = 0;
			virtual void		set_AutoArrange(bool value) = 0;
			virtual bool		get_CheckBox() = 0;
			virtual void		set_CheckBox(bool value) = 0;
			virtual bool		get_FullRowSelect() = 0;
			virtual void		set_FullRowSelect(bool value) = 0;
			virtual bool		get_GridLine() = 0;
			virtual void		set_GridLine(bool value) = 0;
			virtual bool		get_Grouping() = 0;
			virtual void		set_Grouping(bool value) = 0;
#endif // DOXYGEN

			__declspec(property(get=get_SelectedCount                       )) size_t SelectedCount;///< �I�����ꂽ���ڐ�.
			__declspec(property(get=get_Style        , put=set_Style        )) ListStyle Style;		///< �\���X�^�C��.
			__declspec(property(get=get_AutoArrange  , put=set_AutoArrange  )) bool AutoArrange;	///< ��������.
			__declspec(property(get=get_CheckBox     , put=set_CheckBox     )) bool CheckBox;		///< �`�F�b�N�{�b�N�X.
			__declspec(property(get=get_FullRowSelect, put=set_FullRowSelect)) bool FullRowSelect;	///< ��s�I��.
			__declspec(property(get=get_GridLine     , put=set_GridLine     )) bool GridLine;		///< �O���b�h.
			__declspec(property(get=get_Grouping     , put=set_Grouping     )) bool Grouping;		///< �O���[�v.
		};

		/// �c���[�r���[.
		interface __declspec(novtable) ITreeView : ITree
		{
#ifndef DOXYGEN
#endif // DOXYGEN

			/// �w��̏�Ԃ����A�C�e�����A�w��̃C���^�t�F�[�X�ɍ��킹�Ď擾����.
			/// @param ppInterface �擾����C���^�t�F�[�X. IEnumUnknown �̏ꍇ�͕����A����ȊO�͒P���̃A�C�e�����擾����.
			/// @param status �擾����A�C�e���̏��.
			virtual HRESULT GetContents(REFINTF ppInterface, Status status) = 0;
			/// �A�C�e���̏�Ԃ��擾����.
			virtual HRESULT GetStatus(
				IUnknown* item,		///< �Ώۂ̃A�C�e��.
				DWORD*   status		///< �A�C�e���̏��. null�̏ꍇ�͎擾���Ȃ�.
			) = 0;
			/// �A�C�e���̏�Ԃ�ݒ肷��.
			virtual HRESULT SetStatus(
				IUnknown* item,		///< �Ώۂ̃A�C�e��.
				Status   status		///< �A�C�e���̏��.
			) = 0;
		};

		/// �v���r���[.
		interface __declspec(novtable) IPreview : IWindow
		{
			virtual HRESULT GetContents(REFINTF ppInterface) = 0;
			virtual HRESULT SetContents(IUnknown* contents) = 0;
		};

		/// �V�F���X�g���[�W.
		__interface IShellStorage : IUnknown
		{
			HRESULT QueryStream(IStream** ppStream, io::IEntry* pFolder, bool writable);
			HRESULT SyncDescendants(io::IEntry* pFolder);
		};

		/// �t�H���_�r���[.
		interface __declspec(novtable) IShellListView : IListView
		{
			/// �g���n���h����ݒ肷��.
			/// @param which IWindow�ɉ����A
			///              - __uuidof(IShellStorage) : �X�g���[�W.
			/// @param what �i���n���h���I�u�W�F�N�g.
			virtual HRESULT SetExtension(REFGUID which, IUnknown* what) = 0;

#ifndef DOXYGEN
			virtual bool	get_ShowAllFiles() = 0;
			virtual void	set_ShowAllFiles(bool value) = 0;
			virtual bool	get_RenameExtension() = 0;
			virtual void	set_RenameExtension(bool value) = 0;
			virtual string	get_PatternMask() = 0;
			virtual void	set_PatternMask(string value) = 0;
#endif // DOXYGEN

			__declspec(property(get=get_ShowAllFiles, put=set_ShowAllFiles)) bool ShowAllFiles; ///< �B���t�@�C���̕\��.
			__declspec(property(get=get_RenameExtension, put=set_RenameExtension)) bool RenameExtension; ///< ���l�[�����Ɋg���q��I������.
			__declspec(property(get=get_PatternMask, put=set_PatternMask)) string PatternMask; ///< �t�B���^������

			/// �t�H���_���ړ�����.
			virtual HRESULT Go(
				io::IEntry* folder	///< �ړ���̃t�H���_.
			) = 0;
			/// �t�H���_���ړ�����.
			virtual HRESULT Go(
				Direction dir,	///< ����. DirWest=�߂�, DirEast=�i��, DirNorth=�i��.
				int level = 1	///< �i�K.
			) = 0;
			/// ���݂̃t�H���_���擾����.
			virtual HRESULT GetFolder(
				REFINTF ppFolder	///< io::IEntry �܂��� IShellFolder ���擾�ł���.
			) = 0;

			virtual string GetLastStatusText() = 0;
			virtual HRESULT SelectChecked() = 0;
		};

		/// �K�w�\�������A�C�e��.
		__interface ITreeItem : IUnknown
		{
#ifndef DOXYGEN
			string		    get_Name();
			ref<ICommand>	get_Command();
			int			    get_Image();
#endif // DOXYGEN

			__declspec(property(get=get_Name   )) string        Name;    ///< �\���Ɏg���閼�O.
			__declspec(property(get=get_Command)) ref<ICommand> Command; ///< �֘A�t�����Ă���R�}���h.
			__declspec(property(get=get_Image  )) int           Image;   ///< �C���[�W���X�g�C���f�N�X.

			/// ��ʂɕ\������钼�O�ɌĂ΂��.
			/// @result �\�����ω������ꍇ�ɃC���N�������g�����ŏI�X�V����.
			UINT32 OnUpdate();

			/// �q���������Ă���ꍇ��true.
			bool HasChildren();
			size_t GetChildCount();
			/// �q�����擾.
			HRESULT GetChild(REFINTF ppChild, size_t index);
		};

		/// �ҏW�\�ȊK�w�\�������A�C�e��.
		__interface IEditableTreeItem : ITreeItem
		{
#ifndef DOXYGEN
			void set_Name(string value);
			void set_Command(ICommand* value);
			void set_Image(int value);
#endif // DOXYGEN

			__declspec(property(get=get_Name   , put=set_Name   )) string        Name;
			__declspec(property(get=get_Command, put=set_Command)) ref<ICommand> Command;
			__declspec(property(get=get_Image  , put=set_Image  )) int           Image;

			void AddChild(ITreeItem* child);
			bool RemoveChild(ITreeItem* child);
		};

		/// �L�[�}�b�v.
		__interface IKeymap : IUnknown
		{
			/// �L�[�������ꂽ.
			HRESULT OnKeyDown(IWindow* window, UINT16 modifiers, UINT8 vkey);
		};

		/// �R�}���h�o�^�^�̃L�[�}�b�v.
		__interface IKeymapTable : IKeymap
		{
#ifndef DOXYGEN
			size_t get_Count();
#endif // DOXYGEN

			/// �o�C���h�����擾����.
			__declspec(property(get=get_Count)) size_t Count;
			/// �C���f�N�X���w�肵�ăo�C���h�ƃR�}���h���擾����.
			HRESULT GetBind(size_t index, UINT16* modifiers, UINT8* vkey, REFINTF ppCommand);
			/// �o�C���h���w�肵�ăR�}���h���擾����.
			HRESULT GetBind(UINT16  modifiers, UINT8 vkey, REFINTF ppCommand);
			/// �L�[�o�C���h��ǉ�����.
			HRESULT SetBind(UINT16 modifiers, UINT8 vkey, ICommand* pCommand);
		};

		/// �}�E�X�W�F�X�`�����t�b�N����.
		__interface IGesture : IUnknown
		{
			/// 
			HRESULT OnGestureAccept(HWND hWnd, Point ptScreen, size_t length, const Gesture gesture[]);
			/// 
			HRESULT OnGestureUpdate(UINT16 modifiers, size_t length, const Gesture gesture[]);
			/// 
			HRESULT OnGestureFinish(UINT16 modifiers, size_t length, const Gesture gesture[]);
		};

		/// �R�}���h�o�^�^�̃}�E�X�W�F�X�`��.
		__interface IGestureTable : IGesture
		{
			/// �o�C���h�����擾����.
			size_t get_Count();
			/// �C���f�N�X���w�肵�ăR�}���h���擾����.
			HRESULT GetGesture(size_t index, REFINTF ppCommand);
			/// �W�F�X�`�����w�肵�ăR�}���h���擾����.
			HRESULT GetGesture(UINT16 modifiers, size_t length,	const Gesture gesture[], REFINTF ppCommand);
			/// �W�F�X�`����ǉ�����.
			HRESULT SetGesture(UINT16 modifiers, size_t length,	const Gesture gesture[], ICommand* pCommand);
		};

		/// �w�i�摜�T�|�[�g.
		__interface IWallPaper : IUnknown
		{
#ifndef DOXYGEN
			string get_WallPaperFile();
			void   set_WallPaperFile(string value);
			UINT32 get_WallPaperAlign();
			void   set_WallPaperAlign(UINT32 value);
#endif // DOXYGEN

			__declspec(property(get=get_WallPaperFile , put=set_WallPaperFile )) string WallPaperFile;
			__declspec(property(get=get_WallPaperAlign, put=set_WallPaperAlign)) UINT32 WallPaperAlign;
		};

		/// �ǎ��w��_�C�A���O.
		__interface IWallPaperDialog : IUnknown
		{
			HRESULT Go();
			HRESULT AddTarget(IWallPaper* target, string name);
		};

		/// �������ۂ��H
		__interface IExpose : IUnknown
		{
			void AddRect(INT32 id, INT32 group, const Rect& bounds, UINT8 hotkey);
			void Select(UINT32 id);
			void SetTitle(string title);
			HRESULT Go(HWND hwndParent, UINT32 time);
		};

		//==============================================================================
		// ���b�Z�[�W

		/// �E�B���h�E�R�}���h.
		enum WindowCommand
		{
			// standard window
			CommandClose			= 'CLOS', ///< IWindow::Close().
			CommandUpdate			= 'UPDT', ///< IWindow::Update(bool sync = false).
			CommandShow				= 'SHOW', ///< IWindow::set_Visible(bool value = {none=>toggle})
			// editable window
			CommandCut				= 'CUT_', ///< �J�b�g.
			CommandCopy				= 'COPY', ///< �R�s�[.
			CommandPaste			= 'PAST', ///< �\��t��.
			CommandDelete			= 'DELE', ///< �폜.
			CommandBury				= 'BURY', ///< ���S�ɍ폜.
			CommandUndo				= 'UNDO', ///< Undo.
			CommandRedo				= 'REDO', ///< Redo.
			CommandSelectAll		= 'SELA', ///< ���ׂẴA�C�e����I������.
			CommandSelectChecked	= 'SELC', ///< �`�F�b�N���ꂽ�A�C�e����I������.
			CommandSelectNone		= 'SELN', ///< ���ׂẴA�C�e���̑I�����������܂�.
			CommandSelectReverse	= 'SELR', ///< �I�����ڂ𔽓]���܂�.
			CommandSelectToFirst	= 'SELF', ///< �ŏ��܂őI������.
			CommandSelectToLast		= 'SELL', ///< �ŏ��܂őI������.
			CommandCheckAll			= 'CHKA', ///< ���ׂẴA�C�e�����`�F�b�N����.
			CommandCheckNone		= 'CHKN', ///< ���ׂẴA�C�e���̃`�F�b�N���������܂�.
			CommandProperty			= 'PROP', ///< 
			CommandOpen				= 'OPEN', ///< �J��.
			CommandSave				= 'SAVE', ///< �ۑ�.
			// navigating window
			CommandGoBack			= 'GOBK', ///< �߂� or ����.
			CommandGoForward		= 'GOFW', ///< �i�� or �E��.
			CommandGoUp				= 'GOUP', ///< ���.
			CommandGoDown			= 'GODW', ///< ����.
			// form window
			CommandMaximize			= 'MAXI', ///< 
			CommandMinimize			= 'MINI', ///< 
			CommandRestore			= 'REST', ///< 
			CommandResize			= 'RESZ', ///< 
			CommandMove				= 'MOVE', ///< 
			CommandMenu				= 'MENU', ///< 
			//
			CommandKeyUp			= 'KEYU',  ///< �J�[�\����
			CommandKeyDown			= 'KEYD',  ///< �J�[�\����
			CommandKeyLeft			= 'KEYL',  ///< �J�[�\����
			CommandKeyRight			= 'KEYR',  ///< �J�[�\���E
			CommandKeyHome			= 'KEYH',  ///< Home
			CommandKeyEnd			= 'KEYE',  ///< End
			CommandKeyPageUp		= 'KEYP',  ///< PageUp
			CommandKeyPageDown		= 'KEYN',  ///< PageDown
			CommandKeySpace			= 'KEYS',  ///< Space
			CommandKeyEnter			= 'KEYT',  ///< Enter

			CommandShownToLeft		= 'STOL', ///< �^�u������
			CommandShownToRight		= 'STOR', ///< �^�u���E��
			CommandLockedToLeft		= 'LTOL', ///< �^�u������
			CommandLockedToRight	= 'LTOR', ///< �^�u���E��

			CommandSetStyle			= 'STYS', ///< IListView::set_Style(INT32 style)
			CommandRename			= 'RENA', ///< Rename.
			CommandFocusAddress		= 'FCAD', ///< �A�h���X�o�[�Ƀt�H�[�J�X���ڂ�.
			CommandFocusHeader		= 'FCHD', ///< �w�b�_�Ƀt�H�[�J�X���ڂ�.
			CommandAdjustToItem		= 'ADJI', ///< 
			CommandAdjustToWindow	= 'ADJW', ///< 
		};

		/// �E�B���h�E���b�Z�[�W�R�[�h.
		enum WindowMessage
		{
			EventDispose		= 'WDEL', ///< �E�B���h�E���j�����ꂽ.
			EventPreClose		= 'WPCL', ///< �E�B���h�E�̕��悤�Ƃ��Ă���.
			EventClose			= 'WCLS', ///< �E�B���h�E������ꂽ.
			EventRename			= 'WREN', ///< �E�B���h�E�̖��O���ύX���ꂽ.
			EventResize			= 'WRSZ', ///< �E�B���h�E�̃T�C�Y���ύX���ꂽ.
			EventResizeDefault	= 'WRSD', ///< �����T�C�Y���ω�����.
			EventData			= 'DATA', ///< WM_COPYDATA���󂯎����.
			EventUnsupported	= 'UNSP', ///< �����ł��Ȃ����b�Z�[�W.

			EventItemFocus		= 'ITFC', ///< ���ڂ̃t�H�[�J�X��Ԃ��ω�����.
			EventMouseWheel		= 'MSWH', ///< �}�E�X�z�C�[������]����.
			EventOtherFocus		= 'OTHR', ///< ���̃X���b�h�̃E�B���h�E���t�H�[�J�X��������.
		};

		/// �V�F�����X�g���b�Z�[�W�R�[�h.
		enum ShellListMessage
		{
			EventFolderChanging = 'SLCI', ///< �t�H���_���ړ����悤�Ƃ��Ă���.
			EventFolderChange   = 'SLCG', ///< �\���t�H���_���ύX���ꂽ.
			EventExecuteEntry	= 'SLEX', ///< �G���g�������s���悤�Ƃ��Ă���.
			EventStatusText		= 'SLST', ///< �X�e�[�^�X�o�[�̃e�L�X�g���ύX���ꂽ.
		};
	}
}

namespace mew
{
	namespace ui
	{
		HRESULT ProcessDragEnter(IDataObject* src, io::IEntry* dst, DWORD key, DWORD* effect);
		HRESULT ProcessDragOver(io::IEntry* dst, DWORD key, DWORD* effect);
		HRESULT ProcessDragLeave();
		HRESULT ProcessDrop(IDataObject* src, io::IEntry* dst, POINTL pt, DWORD key, DWORD* effect);

		inline bool IsKeyPressed(int vkey)
		{
			return (::GetKeyState(vkey) & 0x8000) != 0;
		}

		inline UINT16 GetCurrentModifiers()
		{
			UINT16 mod = 0;
			if(IsKeyPressed(VK_MENU))    mod |= ModifierAlt;
			if(IsKeyPressed(VK_CONTROL)) mod |= ModifierControl;
			if(IsKeyPressed(VK_SHIFT))   mod |= ModifierShift;
			if(IsKeyPressed(VK_LWIN))    mod |= ModifierWindows;
			if(IsKeyPressed(VK_RWIN))    mod |= ModifierWindows;
			return mod;
		}

		HRESULT QueryInterfaceInWindow(HWND hWnd, REFINTF pp);

		inline HRESULT QueryParent(HWND hWnd, REFINTF pp)
		{
			return QueryInterfaceInWindow(::GetAncestor(hWnd, GA_PARENT), pp);
		}
		inline HRESULT QueryParent(IWindow* window, REFINTF pp)
		{
			if(!window)
				return E_POINTER;
			return QueryParent(window->Handle, pp);
		}

		inline HRESULT QueryParentOrOwner(HWND hWnd, REFINTF pp)
		{
			return QueryInterfaceInWindow(afx::GetParentOrOwner(hWnd), pp);
		}
	}
}

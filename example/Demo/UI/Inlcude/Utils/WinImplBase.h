#ifndef WIN_IMPL_BASE_HPP
#define WIN_IMPL_BASE_HPP

namespace DuiLib
{

	enum UILIB_RESOURCETYPE
	{
		UILIB_FILE=1,				// 来自磁盘文件
		UILIB_ZIP,						// 来自磁盘zip压缩包
		UILIB_RESOURCE,			// 来自资源
		UILIB_ZIPRESOURCE,	// 来自资源的zip压缩包
	};

	class DUILIB_API WindowImplBase
		: public CWindowWnd
		, public CNotifyPump
		, public INotifyUI
		, public IMessageFilterUI
		, public IDialogBuilderCallback
	{
	public:
		WindowImplBase(){};
		virtual ~WindowImplBase(){};
		virtual void InitWindow(){};
		virtual void OnFinalMessage( HWND hWnd );
		virtual void Notify(TNotifyUI& msg);

		DUI_DECLARE_MESSAGE_MAP()
		virtual void OnClick(TNotifyUI& msg);

	protected:
		virtual CDuiString GetSkinFolder() = 0;
		virtual CDuiString GetSkinFile() = 0;
		virtual LPCTSTR GetWindowClassName(void) const = 0 ;
		virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

		CPaintManagerUI m_PaintManager;
		static LPBYTE m_lpResourceZIPBuffer;

	public:
		virtual UINT GetClassStyle() const;
		virtual UILIB_RESOURCETYPE GetResourceType() const;
		virtual CDuiString GetZIPFileName() const;
		virtual LPCTSTR GetResourceID() const;
		virtual CControlUI* CreateControl(LPCTSTR pstrClass);

        virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/);
        virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT ProcessWindowMessage(_In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam, _In_ BOOL& bHandled);

    private:
        LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
#if defined(WIN32) && !defined(UNDER_CE)
		LRESULT OnNcActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
		LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnNcPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
#endif

    public:
        virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
        virtual LONG GetStyle();

	};
}

#endif // WIN_IMPL_BASE_HPP

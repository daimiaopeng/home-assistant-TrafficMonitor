// DrawScrollView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PluginTester.h"
#include "PreviewScrollView.h"
#include "DrawCommon.h"
#include "PluginTesterDlg.h"

// DrawScrollView

IMPLEMENT_DYNCREATE(CDrawScrollView, CScrollView)

CDrawScrollView::CDrawScrollView()
{
}

CDrawScrollView::~CDrawScrollView()
{
}


BEGIN_MESSAGE_MAP(CDrawScrollView, CScrollView)
	ON_WM_ERASEBKGND()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_RBUTTONUP()
    ON_WM_KEYUP()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// DrawScrollView ��ͼ

void CDrawScrollView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

    //��ʼ�������ʾ
    m_tool_tips.Create(this, TTS_ALWAYSTIP | TTS_NOPREFIX);
    m_tool_tips.SetMaxTipWidth(800);		//Ϊ�����ʾ����һ������ȣ��������任��
    m_tool_tips.AddTool(this, _T(""));

    m_size.cx = 0;	
	m_size.cy = 0;	
	SetScrollSizes(MM_TEXT, m_size);
}

void CDrawScrollView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �ڴ���ӻ��ƴ���

	CRect draw_rect(CPoint(0, 0), m_size);
    pDC->FillSolidRect(draw_rect, RGB(222, 238, 255));

    //˫�����ͼ
    CDrawDoubleBuffer draw_double_buffer(pDC, draw_rect);
    //m_draw_rect = draw_rect;

    CDrawCommon drawer;
    drawer.Create(draw_double_buffer.GetMemDC(), this);

    draw_rect.MoveToY(0);
    drawer.FillRect(draw_rect, GetSysColor(COLOR_WINDOW));
    drawer.SetFont(theApp.m_pMainWnd->GetFont());

    //���Ʋ������ʾ��Ŀ
    int index = 0;

    CPluginTesterDlg* dlg = dynamic_cast<CPluginTesterDlg*>(theApp.m_pMainWnd);
    if (dlg == nullptr)
        return;

    PluginInfo cur_plugin = dlg->GetCurrentPlugin();
    for (const auto& item : cur_plugin.plugin_items)
    {
        if (item != nullptr)
        {
            bool dark_mode = dlg->IsDarkmodeChecked();
            COLORREF text_color = (dark_mode ? RGB(255, 255, 255) : RGB(0, 0, 0));
            COLORREF back_color = (dark_mode ? RGB(24, 37, 55) : RGB(205, 221, 234));

            bool double_line = dlg->IsDoubleLineChecked();

            //�����ı�
            CRect rc_text;
            rc_text.left = theApp.DPI(16);
            rc_text.top = theApp.DPI(8) + index * (double_line ? theApp.DPI(64) : theApp.DPI(56));
            rc_text.bottom = rc_text.top + theApp.DPI(24);
            rc_text.right = rc_text.left + theApp.DPI(120);
            drawer.DrawWindowText(rc_text, item->GetItemName(), RGB(0, 0, 0));

            //������ʾ��Ŀ
            CRect rc_item = rc_text;
            if (double_line)
                rc_item.bottom += theApp.DPI(10);
            rc_item.MoveToY(rc_text.bottom);
            int item_width = dlg->GetItemWidth(item, drawer.GetDC());
            if (item_width > 0)
                rc_item.right = rc_item.left + item_width;
            drawer.FillRect(rc_item, back_color);

            //������ʾ��Ŀλ��
            m_plugin_item_rect[item->GetItemId()] = rc_item;

            //�����ǰ��ͼ�Ŀ��С����ʾ��Ŀ�Ŀ�ȣ���������ͼ�Ŀ��
            if (m_size.cx < rc_item.Width() + theApp.DPI(32))
            {
                m_size.cx = rc_item.Width() + theApp.DPI(32);
                SetScrollSizes(MM_TEXT, m_size);
            }

            //������ʾ��Ŀ��λ��
            //SavePluginItemRect(item, rc_item);

            if (item->IsCustomDraw())
            {
                drawer.GetDC()->SetTextColor(text_color);
                item->DrawItem(drawer.GetDC()->GetSafeHdc(), rc_item.left, rc_item.top, rc_item.Width(), rc_item.Height(), dark_mode);
            }
            else
            {
                CRect rc_label = rc_item;
                CRect rc_value = rc_item;

                if (double_line)
                {
                    rc_label.bottom = rc_label.top + (rc_label.Height() / 2);
                    rc_value.top = rc_label.bottom;
                }
                else
                {
                    rc_label.right = rc_label.left + drawer.GetDC()->GetTextExtent(item->GetItemLableText()).cx;
                    rc_value.left = rc_label.right + theApp.DPI(6);
                }

                drawer.DrawWindowText(rc_value, item->GetItemValueText(), text_color);
                drawer.DrawWindowText(rc_label, item->GetItemLableText(), text_color);
            }
            index++;
        }
    }
}


// DrawScrollView ���
#ifdef _DEBUG
void CDrawScrollView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CDrawScrollView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG

void CDrawScrollView::InitialUpdate()
{
	OnInitialUpdate();
}

void CDrawScrollView::SetSize(int width,int hight)
{
	SetSize(CSize(width, hight));
}



void CDrawScrollView::SetSize(CSize size)
{
    m_size = size;
    SetScrollSizes(MM_TEXT, m_size);
}


void CDrawScrollView::PluginChanged()
{
    m_plugin_item_clicked = nullptr;
}


CSize CDrawScrollView::GetSize() const
{
    return m_size;
}

void CDrawScrollView::UpdateMouseToolTip()
{
    CString tip_info;
    CPluginTesterDlg* dlg = dynamic_cast<CPluginTesterDlg*>(theApp.m_pMainWnd);
    if (dlg != nullptr)
    {
        ITMPlugin* plugin = dlg->GetCurrentPlugin().plugin;
        if (plugin != nullptr)
        {
            tip_info = plugin->GetTooltipInfo();
            m_tool_tips.UpdateTipText(tip_info, this);
        }
    }
}

IPluginItem* CDrawScrollView::GetPluginItemByPoint(CPoint point)
{
    CPluginTesterDlg* dlg = dynamic_cast<CPluginTesterDlg*>(theApp.m_pMainWnd);
    if (dlg != nullptr)
    {
        PluginInfo plugin_info = dlg->GetCurrentPlugin();
        for (IPluginItem* plugin : plugin_info.plugin_items)
        {
            if (plugin != nullptr)
            {
                CRect rc_item = m_plugin_item_rect[plugin->GetItemId()];
                if (rc_item.PtInRect(point))
                    return plugin;
            }
        }
    }
    return nullptr;
}

BOOL CDrawScrollView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rect;
	CBrush brush;
	brush.CreateSolidBrush(RGB(235,235,235));
	pDC->GetClipBox(rect);
	pDC->FillRect(rect, &brush);
	return TRUE;

	//return CScrollView::OnEraseBkgnd(pDC);
}


void CDrawScrollView::OnLButtonUp(UINT nFlags, CPoint point)
{
    SetFocus();
    m_plugin_item_clicked = GetPluginItemByPoint(point);
    if (m_plugin_item_clicked != nullptr)
    {
        CRect rc_item = m_plugin_item_rect[m_plugin_item_clicked->GetItemId()];
        m_plugin_item_clicked->OnMouseEvent(IPluginItem::MT_LCLICKED, point.x - rc_item.left, point.y - rc_item.top, GetSafeHwnd(), IPluginItem::MF_TASKBAR_WND);
        InvalidateRect(rc_item);
    }
    else
    {
        CScrollView::OnLButtonUp(nFlags, point);
    }
}


void CDrawScrollView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    SetFocus();
    m_plugin_item_clicked = GetPluginItemByPoint(point);
    if (m_plugin_item_clicked != nullptr)
    {
        CRect rc_item = m_plugin_item_rect[m_plugin_item_clicked->GetItemId()];
        m_plugin_item_clicked->OnMouseEvent(IPluginItem::MT_DBCLICKED, point.x - rc_item.left, point.y - rc_item.top, GetSafeHwnd(), IPluginItem::MF_TASKBAR_WND);
        InvalidateRect(rc_item);
    }
    else
    {
        CScrollView::OnLButtonDblClk(nFlags, point);
    }
}


void CDrawScrollView::OnRButtonUp(UINT nFlags, CPoint point)
{
    SetFocus();
    m_plugin_item_clicked = GetPluginItemByPoint(point);
    if (m_plugin_item_clicked != nullptr)
    {
        CRect rc_item = m_plugin_item_rect[m_plugin_item_clicked->GetItemId()];
        m_plugin_item_clicked->OnMouseEvent(IPluginItem::MT_RCLICKED, point.x - rc_item.left, point.y - rc_item.top, GetSafeHwnd(), IPluginItem::MF_TASKBAR_WND);
        InvalidateRect(rc_item);
    }
    else
    {
        CScrollView::OnRButtonUp(nFlags, point);
    }
}


BOOL CDrawScrollView::PreTranslateMessage(MSG* pMsg)
{
    // TODO: �ڴ����ר�ô����/����û���
    if (pMsg->message == WM_KEYDOWN)
    {
        CPluginTesterDlg* dlg = dynamic_cast<CPluginTesterDlg*>(theApp.m_pMainWnd);
        if (dlg != nullptr)
        {
            PluginInfo plugin_info = dlg->GetCurrentPlugin();
            IPluginItem* plugin_item = m_plugin_item_clicked;
            if (plugin_item == nullptr && !plugin_info.plugin_items.empty())
                plugin_item = plugin_info.plugin_items.front();
            if (plugin_info.plugin != nullptr && plugin_info.plugin->GetAPIVersion() >= 4 && plugin_item != nullptr)
            {
                bool ctrl = (GetKeyState(VK_CONTROL) & 0x80);
                bool shift = (GetKeyState(VK_SHIFT) & 0x8000);
                bool alt = (GetKeyState(VK_MENU) & 0x8000);
                int rtn = plugin_item->OnKeboardEvent(pMsg->wParam, ctrl, shift, alt, (void*)GetSafeHwnd(), IPluginItem::KF_TASKBAR_WND);
                CRect rc_item = m_plugin_item_rect[plugin_item->GetItemId()];
                InvalidateRect(rc_item);
                if (rtn != 0)
                    return TRUE;
            }
        }
    }

    if (m_tool_tips.GetSafeHwnd() != 0)
    {
        m_tool_tips.RelayEvent(pMsg);
    }

    return CScrollView::PreTranslateMessage(pMsg);
}


void CDrawScrollView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}

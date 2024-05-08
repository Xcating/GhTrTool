// InfoDialog.cpp: 实现文件
//

#include "stdafx.h"
#include "GhTrTool.h"
#include "afxdialogex.h"
#include "InfoDialog.h"


// InfoDialog 对话框

IMPLEMENT_DYNAMIC(InfoDialog, CDialogEx)

InfoDialog::InfoDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PLANTID, pParent)
{

}

InfoDialog::~InfoDialog()
{
}
BOOL InfoDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
    // 定义植物名称和ID
    struct Plant {
        CString name;
        CString id;
    };
    Plant plants[] = {
        {_T("豌豆射手"), _T("0")},
        {_T("向日葵"), _T("1")},
        {_T("樱桃炸弹"), _T("2")},
        {_T("坚果墙"), _T("3")},
        {_T("荷叶"), _T("4")},
        {_T("车前草"), _T("5")},
        {_T("猫丝子"), _T("6")},
        {_T("荷鲁斯"), _T("7")},
        {_T("光菱角"), _T("8")},
        {_T("白隅兰"), _T("9")},
        {_T("闪电芦苇"), _T("10")},
        {_T("苹果鼓瑟手"), _T("11")},
        {_T("滇池牡丹"), _T("12")},
        {_T("导藓"), _T("13")},
        {_T("土豆地雷"), _T("14")},
        {_T("淀粉海"), _T("15")},
    };
    // 插入列
    m_List.InsertColumn(0, _T("植物名称"), LVCFMT_LEFT, 100);
    m_List.InsertColumn(1, _T("ID"), LVCFMT_LEFT, 50);

    // 循环插入植物数据
    for (int i = 0; i < sizeof(plants) / sizeof(plants[0]); ++i) {
        int index = m_List.InsertItem(i, plants[i].name);
        m_List.SetItemText(index, 1, plants[i].id);
    }

    // 定义难度名称和ID
    struct Difficulty {
        CString name;
        CString id;
    };
    Plant Difficulty[] = {
        {_T("简单 Easy"), _T("0")},
        {_T("正常 Medium"), _T("1")},
        {_T("较难 Hard"), _T("2")},
        {_T("特难 Impossible"), _T("3")},
        {_T("失衡 Unbalanced"), _T("4")},
    };
    // 插入列
    m_List2.InsertColumn(0, _T("难度"), LVCFMT_LEFT, 100);
    m_List2.InsertColumn(1, _T("ID"), LVCFMT_LEFT, 50);

    // 循环插入植物数据
    for (int i = 0; i < sizeof(Difficulty) / sizeof(Difficulty[0]); ++i) {
        int index = m_List2.InsertItem(i, Difficulty[i].name);
        m_List2.SetItemText(index, 1, Difficulty[i].id);
    }
    // 定义背景名称和ID
    struct BackGround {
        CString name;
        CString id;
    };
    Plant BackGround[] = {
         {_T("潜意识场景"), _T("0")},
        {_T("逻辑场景(无贴图)"), _T("1")},
        {_T("瀑布入口"), _T("2")},
        {_T("枫威瀑布"), _T("3")},
        {_T("枫威湖"), _T("4")},
        {_T("枫威水帘洞"), _T("5")},
        {_T("迷蒙之沼"), _T("6")},
        {_T("迷蒙神隐"), _T("7")},
        {_T("绽凌原野"), _T("8")},
    };
    // 插入列
    m_List3.InsertColumn(0, _T("背景名称"), LVCFMT_LEFT, 100);
    m_List3.InsertColumn(1, _T("ID"), LVCFMT_LEFT, 50);

    // 循环插入植物数据
    for (int i = 0; i < sizeof(BackGround) / sizeof(BackGround[0]); ++i) {
        int index = m_List3.InsertItem(i, BackGround[i].name);
        m_List3.SetItemText(index, 1, BackGround[i].id);
    }
	return TRUE;  // return TRUE unless you set the focus to a control
}
void InfoDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_List);
    DDX_Control(pDX, IDC_LIST2, m_List2);
    DDX_Control(pDX, IDC_LIST3, m_List3);
}


BEGIN_MESSAGE_MAP(InfoDialog, CDialogEx)
END_MESSAGE_MAP()


// InfoDialog 消息处理程序

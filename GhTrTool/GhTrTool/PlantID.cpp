// PlantID.cpp: 实现文件
//

#include "stdafx.h"
#include "GhTrTool.h"
#include "afxdialogex.h"
#include "PlantID.h"


// PlantID 对话框

IMPLEMENT_DYNAMIC(PlantID, CDialogEx)

PlantID::PlantID(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PLANTID, pParent)
{

}

PlantID::~PlantID()
{
}
BOOL PlantID::OnInitDialog()
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
        {_T("猫丝子"), _T("5")},
        {_T("荷鲁斯"), _T("6")},
        {_T("光菱角"), _T("7")},
        {_T("白隅兰"), _T("8")},
        {_T("闪电芦苇"), _T("9")},
        {_T("苹果鼓瑟手"), _T("10")},
        {_T("滇池牡丹"), _T("11")},
        {_T("导藓"), _T("12")},
        {_T("土豆地雷"), _T("13")},
        {_T("梅花拖鞋"), _T("14")}
    };
    // 插入列
    m_List.InsertColumn(0, _T("植物名称"), LVCFMT_LEFT, 100);
    m_List.InsertColumn(1, _T("ID"), LVCFMT_LEFT, 50);

    // 循环插入植物数据
    for (int i = 0; i < sizeof(plants) / sizeof(plants[0]); ++i) {
        int index = m_List.InsertItem(i, plants[i].name);
        m_List.SetItemText(index, 1, plants[i].id);
    }
	return TRUE;  // return TRUE unless you set the focus to a control
}
void PlantID::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(PlantID, CDialogEx)
END_MESSAGE_MAP()


// PlantID 消息处理程序

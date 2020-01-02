#include "remotemanagementpanel.h"
#include "serverconfigitem.h"
RemoteManagementPanel::RemoteManagementPanel(QWidget *parent) : CommonPanel(parent)
{
    initUI();

}
void RemoteManagementPanel::refreshPanel()
{
    clearSearchInfo();
    m_listWidget->refreshAllDatas();
    refreshSearchState();
}
void RemoteManagementPanel::refreshSearchState()
{
    if (m_listWidget->count() >= 2) {
        m_searchEdit->show();
    } else {
        m_searchEdit->hide();
    }
}

void RemoteManagementPanel::showCurSearchResult()
{
    QString strTxt = m_searchEdit->text();
    if (strTxt.isEmpty())
        return;
    emit showSearchPanel(strTxt);
}

void RemoteManagementPanel::showAddServerConfigDlg()
{
#if 0
    CustomCommandOptDlg dlg(CustomCommandOptDlg::CCT_ADD, nullptr, this);
    if (dlg.exec() == QDialog::Accepted) {
        QAction &newAction = dlg.getCurCustomCmd();
        QAction *existAction = ShortcutManager::instance()->checkActionIsExist(newAction);
        if (nullptr == existAction) {
            QAction *newTmp =  ShortcutManager::instance()->addCustomCommand(newAction);
            m_listWidget->addOneRowData(newTmp);
            refreshSearchState();
        } else {
            existAction->data() = newAction.data();
            existAction->setShortcut(newAction.shortcut());
            m_listWidget->refreshOneRowCommandInfo(existAction);
            ShortcutManager::instance()->saveCustomCommandToConfig(existAction);
        }

    }
#endif
}


void RemoteManagementPanel::initUI()
{
    //setAttribute(Qt::WA_TranslucentBackground);//Qt::WA_NoBackground|
    m_searchEdit = new DSearchEdit(this);
    m_listWidget = new ServerConfigList(this);
    m_pushButton = new DPushButton(this);

    m_searchEdit->setClearButtonEnabled(true);
    //m_searchEdit->setFixedHeight(40);
    //m_searchEdit->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
    m_searchEdit->setStyleSheet("border-width:1;border-style:none;"
                                //"background-color: transparent;"
                               );//"font-color:white;"


    m_listWidget->setSelectionMode(QAbstractItemView::NoSelection);
    m_listWidget->setVerticalScrollMode(QAbstractItemView::ScrollMode::ScrollPerItem);
    m_listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_listWidget->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
    //setStyleSheet("QListWidget::item:hover{background-color:rgb(0,255,0,50)}"
    //             "QListWidget::item:selected{background-color:rgb(255,0,0,100)}");
    m_listWidget->setStyleSheet("QListWidget{border:1px solid gray; color:black; }"
                                "QListWidget::item:hover{background-color:lightgray}"
                                "QListWidget::item:selected{background:lightgray; color:red; }"
                                "QListWidget::item:selected{background-color:rgb(255,0,0,100)}"
                                "QListWidget::item:border_style{solid,none,solid,solid}"
                                "QListWidget::item:border_width{medium,none,medium,none}"
                                "QListWidget::item:border_color{gray,none,gray,red}");

    m_pushButton->setFixedHeight(60);
    m_pushButton->setText("+ Add Command");
    m_pushButton->setStyleSheet("border-width:0;border-style:sloid");
    //connect(m_iconButton, &DIconButton::clicked, this, &CommonPanel::iconButtonCliecked);//
    //connect(m_searchEdit, &DSearchEdit::returnPressed, this, &CommonPanel::searchEditingFinished);//
    // connect(m_pushButton, &DPushButton::clicked, this, &CommonPanel::pushButtonClicked);
    //connect(m_listWidget, &DListWidget::itemClicked, this, &CommonPanel::listWidgetItemClicked);

    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->addWidget(m_searchEdit);
    hlayout->setSpacing(0);
    hlayout->setMargin(0);

    QVBoxLayout *vlayout = new QVBoxLayout();
    vlayout->addLayout(hlayout);
    vlayout->addWidget(m_listWidget);
    vlayout->addWidget(m_pushButton);
    vlayout->setMargin(0);
    vlayout->setSpacing(0);
    setLayout(vlayout);

    connect(m_searchEdit, &DSearchEdit::returnPressed, this, &RemoteManagementPanel::showCurSearchResult);//
    connect(m_pushButton, &DPushButton::clicked, this, &RemoteManagementPanel::showAddServerConfigDlg);
    connect(m_listWidget, &DListWidget::itemClicked, this, &RemoteManagementPanel::listItemClicked);
    connect(m_listWidget, &ServerConfigList::listItemCountChange, this, &RemoteManagementPanel::refreshSearchState);
}

void RemoteManagementPanel::listItemClicked(QListWidgetItem *item)
{
    ServerConfigItem *widgetTemp = qobject_cast<ServerConfigItem *>(m_listWidget->itemWidget(item));
    if (nullptr == widgetTemp) {
        return;
    }
    if (widgetTemp->isGroup()) {
        emit showServerConfigGroupPanel(widgetTemp->getGroupName());
    } else {
        emit doConnectServer();
    }
}

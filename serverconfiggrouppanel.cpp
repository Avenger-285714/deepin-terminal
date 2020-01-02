#include "serverconfiggrouppanel.h"
#include "serverconfigitem.h"

ServerConfigGroupPanel::ServerConfigGroupPanel(QWidget *parent) : CommonPanel(parent)
{
    initUI();
}

void ServerConfigGroupPanel::initUI()
{
    m_iconButton = new DIconButton(this);
    m_searchEdit = new DSearchEdit(this);
    m_listWidget = new ServerConfigList(this);

    m_iconButton->setIcon(DStyle::StandardPixmap::SP_ArrowPrev);
    m_iconButton->setFixedSize(QSize(40, 40));
    m_iconButton->setStyleSheet("border-width:0;border-style:none;");
    m_searchEdit->setFixedWidth(210);
    m_searchEdit->setClearButtonEnabled(true);
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


    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->addWidget(m_iconButton);
    hlayout->addWidget(m_searchEdit);
    hlayout->addStretch();
    hlayout->setSpacing(0);
    hlayout->setMargin(0);

    QVBoxLayout *vlayout = new QVBoxLayout();
    vlayout->addLayout(hlayout);
    vlayout->addWidget(m_listWidget);
    vlayout->setMargin(0);
    vlayout->setSpacing(0);
    setLayout(vlayout);

    connect(m_searchEdit, &DSearchEdit::returnPressed, this, &ServerConfigGroupPanel::handleShowSearchResult);//
    connect(m_listWidget, &DListWidget::itemClicked, this, &ServerConfigGroupPanel::doConnectServer);
    connect(m_iconButton, &DIconButton::clicked, this, &ServerConfigGroupPanel::showRemoteManagementPanel);
    connect(m_listWidget, &ServerConfigList::listItemCountChange, this, &ServerConfigGroupPanel::refreshSearchState);
}
void ServerConfigGroupPanel::refreshData(const QString &groupName)
{
    m_groupName = groupName;
    m_listWidget->clear();
    m_listWidget->refreshDataByGroup(groupName);
    refreshSearchState();
}
void ServerConfigGroupPanel::handleShowSearchResult()
{
    QString strFilter = m_searchEdit->text();
    emit showSearchResult(m_groupName, strFilter);
}
void ServerConfigGroupPanel::refreshSearchState()
{
    if (m_listWidget->count() >= 2) {
        m_searchEdit->show();
    } else {
        m_searchEdit->hide();
    }
}

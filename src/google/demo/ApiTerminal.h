#pragma once

#include <iostream>
#include <functional>
#include "google/endpoint/ApiUtil.h"

std::ostream& operator << (std::ostream& os, const QString& s);

namespace googleQt{
    namespace demo{
        class Terminal
        {
        public:
            struct Selection
            {
                QString                         name;
                QString                         description;
                std::function<void(QString)>    action;
            };
            typedef std::list<Selection> SELECTION_LIST;
            typedef qstring_hash_map<Selection> SELECTION_MAP;

Terminal(QString prompt):m_prompt(prompt){};

            void addAction(QString name, QString description, std::function<void(QString)> action)
            {
                Selection s;
                s.name = name;
                s.description = description;
                s.action = action;
                m_sel.push_back(s);
                m_sel_map[name.toLower()] = s;
            };

            void addSeparator()
            {
                addAction("       ", "        ", [](QString){});
            };
            
            void start()
            {
                std::string exit_option("x");
                std::string repeat_option("r");

                while(true)
                    {
                        std::cout << std::endl;
                        std::cout << "-----------------------------------------" << std::endl;

                        for(SELECTION_LIST::iterator i = m_sel.begin(); i != m_sel.end(); i++){
                            QString name = i->name;
                            if (name.indexOf("    ") == 0)
                                {
                                    std::cout << std::endl;
                                    continue;
                                }
                            std::cout << " " << pad(name.toStdString(), 20) << " " << i->description << std::endl;
                        }
                        std::cout << " " << pad(exit_option, 20) << " " << "Exit terminal" << std::endl;
                        if (!m_last_cmd.isEmpty()) {
                            std::cout << " " << pad(repeat_option, 20) << " " << "Repeat last command" << std::endl;
                        }
                        std::cout << std::endl << " " << m_prompt << "> ";

                        std::string tmp;            
                        getline(std::cin, tmp);
                        QString str_cmd = tmp.c_str();

                        if (str_cmd.compare(repeat_option.c_str(), Qt::CaseInsensitive) == 0)
                        {
                            str_cmd = m_last_cmd;
                        }
                                                   
                        QStringList arg_list = str_cmd.split(" ", Qt::SkipEmptyParts);
                        if(!arg_list.empty()){
                            QString str = arg_list[0];
                            if(str.compare(exit_option.c_str(), Qt::CaseInsensitive) == 0)
                                break;

                            arg_list.removeFirst();
                            QString arg = arg_list.join(" ");

                            SELECTION_MAP::iterator i = m_sel_map.find(str);
                            if(i == m_sel_map.end()){
                                std::cout << "invalid command: " << str << std::endl;
                            }
                            else{
                                i->second.action(arg);
                                m_last_cmd = str_cmd;
                            }
                        }
                    }
            };

            static std::string pad(std::string s, size_t num, char paddingChar = ' ')
            {
                std::string str = s;
                if(num > str.size())
                    str.insert(str.size(), num - str.size(), paddingChar);
                return str;
            }

            static QString pad(QString s, size_t num, char paddingChar = ' ')
            {
                return s.leftJustified(static_cast<int>(num), paddingChar, false);
            }
            
            static QString pad(const char* s, size_t num, char paddingChar = ' ')
            {
                return pad(QString(s), num, paddingChar);
            }
            
            static QString pad_trunc(QString s, size_t num, char paddingChar = ' ')
            {
                return s.leftJustified(static_cast<int>(num), paddingChar, true);
            }
            
        protected:
            SELECTION_LIST  m_sel;
            SELECTION_MAP   m_sel_map;
            QString         m_prompt;
            QString         m_last_cmd;
        };
    }//demo
};//googleQt





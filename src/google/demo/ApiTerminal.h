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
                QString							name;
                QString							description;
                std::function<void(QString)>	action;
            };
            typedef std::list<Selection> SELECTION_LIST;
            typedef std::map<QString, Selection> SELECTION_MAP;

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

            void start()
            {
                std::string exit_option("exit");

                while(true)
                    {
                        std::cout << std::endl;
                        std::cout << "-----------------------------------------" << std::endl;

                        for(SELECTION_LIST::iterator i = m_sel.begin(); i != m_sel.end(); i++){
                            QString name = i->name;
                            if (name.indexOf("-----") == 0)
                                {
                                    std::cout << std::endl;
                                    continue;
                                }
                            std::cout << " " << pad(name.toStdString(), 20) << " " << i->description << std::endl;
                        }
                        std::cout << " " << pad(exit_option, 20) << " " << "Exit terminal" << std::endl << std::endl;
std::cout << " " << m_prompt << "> ";

                        std::string tmp;			
                        getline(std::cin, tmp);
                        QString str = tmp.c_str();
                        QStringList arg_list = str.split(" ", QString::SkipEmptyParts);
                        if(!arg_list.empty()){
                            str = arg_list[0];
                            if(str.compare("exit", Qt::CaseInsensitive) == 0)
                                break;

                            arg_list.removeFirst();
                            QString arg = arg_list.join(" ");

                            SELECTION_MAP::iterator i = m_sel_map.find(str);
                            if(i == m_sel_map.end()){
                                std::cout << "invalid command: " << str << std::endl;
                            }
                            else{
                                i->second.action(arg);
                            }
                        }
                    }
            };

            std::string pad(std::string s, const size_t num, const char paddingChar = ' ')
            {
                std::string str = s;
                if(num > str.size())
                    str.insert(str.size(), num - str.size(), paddingChar);
                return str;
            }

        protected:
            SELECTION_LIST  m_sel;
            SELECTION_MAP   m_sel_map;
            QString         m_prompt;
        };
    }//demo
};//googleQt





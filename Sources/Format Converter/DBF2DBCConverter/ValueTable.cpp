/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file      ValueTable.cpp
 * \brief     Implementation file for the value table.This is atable containing value discriptors.
 * \authors   Ratnadip Choudhury, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for the value table.This is atable containing value discriptors.
 */

/* Project inclues */
#include "Converter.h"
#include "Signal.h"
#include "ValueTable.h"

/**
 * \brief Constructor
 *
 * Constructor of CValueTable
 */
CValueTable::CValueTable()
{
    m_TableName[0] = '\0';
}

/**
 * \brief Destructor
 *
 * Destructor of CValueTable
 */
CValueTable::~CValueTable()
{
}

/**
 * \brief     overloaded operator =
 * \param[in] Tab Object to copy data from
 * \return    Local object with new contents
 *
 * Copy contents from Tab to local object.
 */
CValueTable& CValueTable::operator=(CValueTable& Tab)
{
    m_TableName = Tab.m_TableName;
    POSITION posMsg = Tab.m_values.GetHeadPosition();

    while(posMsg != NULL)
    {
        CValueDescriptor& vDisp = Tab.m_values.GetNext(posMsg);
        m_values.AddTail(vDisp );
    }

    return (*this);
}

/**
 * \brief     extracts Values and value descriptors from the line
 * \param[in] pcLine String having the value table name followed by the value, value descriptor as in canoe format
 * \param[in] fileInput Input file
 *
 * extracts Values and value descriptors from the line
 */
void CValueTable::Format_ValueTable(char* pcLine, fstream& fileInput)
{
    char* pcToken;
    char acLine[defCON_MAX_LINE_LEN] = {'\0'};

    //skip all balnk spaces.
    while(*pcLine && *pcLine==' ')
    {
        *pcLine++;
    }

    pcToken=strtok(pcLine,"\n");
    //get value table name.
    m_TableName = pcToken;

    //parses value table.
    while(fileInput.getline(acLine, defCON_MAX_LINE_LEN) && strcmp(acLine,"[END_TABLE]\n")!=0)
    {
        pcLine=acLine;

        //skip white spaces
        while(*pcLine && *pcLine==' ')
        {
            *pcLine++;
        }

        //get value descriptor.
        pcToken=strtok(pcLine," ");

        if(strcmp(pcToken,"[VALUE_DESCRIPTION]")==0)
        {
            pcToken=strtok(NULL,"\"");
            CValueDescriptor valDesc;
            //get the descriptor value.
            valDesc.m_sDescriptor =pcToken;
            pcToken=strtok(NULL,",");
            valDesc.m_value.i64Value = atoi(pcToken);
            m_values.AddTail(valDesc);
        }
    }
}

/**
 * \brief     writes the value tebles in the given list to the output file.
 * \param[in] fileOutput Pointer to the Output file
 * \param[in] vTab List of Value tables
 *
 * writes the value tebles in the given list to the output file.
 */
void CValueTable::writeValueTabToFile(fstream& fileOutput,CList<CValueTable,CValueTable&> &vTab)
{
    //get value table.
    POSITION pos = vTab.GetHeadPosition();

    //repeats till value tables exists in the list.
    while(pos != NULL)
    {
        //write value table name to the o/p file.
        CValueTable& tab = vTab.GetNext(pos);
        fileOutput << "VAL_TABLE_ " << tab.m_TableName << " ";
        //writes descriptors values to the o/p file.
        CValueDescriptor desc;
        desc.writeValuDescToFile (fileOutput,CSignal::SIG_TYPE_INT64,tab.m_values);
    }
}

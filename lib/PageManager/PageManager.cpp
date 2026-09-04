//-------------------------------------------------------------
// File : PageManager.cpp
// Author : JongOh Kim + ChatGPT
// Date : 2026-07-25
// Project : SVEMS
// Version : 0.4.0
// Description : PageManager
//-------------------------------------------------------------

#include "PageManager.h"

namespace PageManager
{
    void Manager::Begin()
    {
        m_currentPage =
            // DisplayPages::Page::Overview;
            DisplayPages::Page::Overview;

            m_subPage =
                0U;
    }

    DisplayPages::Page Manager::Current() const
    {
        return m_currentPage;
    }

    void Manager::Next()
    {
        Serial.printf(
            "[PAGE] Next() current=%u\n",
            static_cast<uint8_t>(m_currentPage)
        );
        
        uint8_t page =
            static_cast<uint8_t>(m_currentPage);

        page++;

        if(page >= static_cast<uint8_t>(DisplayPages::Page::Count))
        {
            page = 0;
        }

        // uint8_t page =
        //     ToIndex(m_currentPage);
        m_currentPage =
            static_cast<DisplayPages::Page>(page);

        m_subPage = 0U;

        Serial.printf(
            "[PAGE] -> %u\n",
            static_cast<uint8_t>(m_currentPage)
        );
    }

    void Manager::Previous()
    {
        Serial.printf(
            "[PAGE] Previous() current=%u\n",
            static_cast<uint8_t>(m_currentPage)
        );

        int page =
            static_cast<uint8_t>(m_currentPage);

        page--;

        if(page < 0)
        {
            page = 
                static_cast<int>(DisplayPages::Page::Count) - 1;
        }

        // uint8_t page =
        //     ToIndex(m_currentPage);
        m_currentPage =
            static_cast<DisplayPages::Page>(page);

        m_subPage = 0U;

        Serial.printf(
            "[PAGE] -> %u\n",
            static_cast<uint8_t>(m_currentPage)
        );
    }

    uint8_t Manager::SubPage() const
    {
        return m_subPage;
    }

    void Manager::NextSubPage()
    {
        const uint8_t count =
            DisplayPages::GetSubPageCount(
                m_currentPage);

        if (count <= 1U)
        {
            m_subPage = 0U;
            return;
        }

        ++m_subPage;

        if (m_subPage >= count)
        {
            m_subPage = 0U;
        }
    }

    void Manager::SetPage(
        DisplayPages::Page page)
    {
        Serial.printf(
            "[PAGE] SetPage() %u -> %u\n",
            static_cast<uint8_t>(m_currentPage),
            static_cast<uint8_t>(page)
        );

        m_currentPage =
            page;

        m_subPage =
            0U;
    }

    void Manager::ResetSubPage()
    {
        m_subPage =
            0U;
    }
}
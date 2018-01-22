#include "project_tree_renderer.hpp"

namespace MinIDE
{
//#####################################################################################################################
    ProjectTreeRenderer::ProjectTreeRenderer(cloneable_renderer const& renderer)
        : renderer_{renderer}
    {
    }
//---------------------------------------------------------------------------------------------------------------------
    void ProjectTreeRenderer::begin_paint(nana::widget& w)
    {
        renderer_->begin_paint(w);
    }
//---------------------------------------------------------------------------------------------------------------------
    void ProjectTreeRenderer::imbueTheme(Theme const& theme)
    {
        theme_ = theme;
    }
//---------------------------------------------------------------------------------------------------------------------
    void ProjectTreeRenderer::bground(graph_reference graph, compset_interface const* compset) const
    {
        comp_attribute_t attr;

        if(compset->comp_attribute(component::bground, attr))
        {
            const nana::color colors[][2] = {
                { theme_.projTree.highlightGradient1, theme_.projTree.highlightGradient2 }, //highlighted
                { theme_.projTree.selectGradient1, theme_.projTree.selectGradient2 }, //Selected and highlighted
                { theme_.projTree.shGradient1, theme_.projTree.shGradient2 }  //Selected but not highlighted
            };

            const nana::color *clr = nullptr;
            if(compset->item_attribute().mouse_pointed)
            {
                if(compset->item_attribute().selected)
                    clr = colors[1];
                else
                    clr = colors[0];
            }
            else if(compset->item_attribute().selected)
                clr = colors[2];

            if (clr)
            {
                graph.rectangle(attr.area, false, clr[1]);
                graph.gradual_rectangle(attr.area.pare_off(1), *clr, *(clr+1), true);
            }
        }
    }
//---------------------------------------------------------------------------------------------------------------------
    void ProjectTreeRenderer::expander(graph_reference graph, const compset_interface * compset) const
    {
        renderer_->expander(graph, compset);
    }
//---------------------------------------------------------------------------------------------------------------------
    void ProjectTreeRenderer::crook(graph_reference graph, const compset_interface * compset) const
    {
        renderer_->crook(graph, compset);
    }
//---------------------------------------------------------------------------------------------------------------------
    void ProjectTreeRenderer::icon(graph_reference graph, const compset_interface * compset) const
    {
        renderer_->icon(graph, compset);
    }
//---------------------------------------------------------------------------------------------------------------------
    void ProjectTreeRenderer::text(graph_reference graph, const compset_interface * compset) const
    {
        comp_attribute_t attr;

        if(compset->comp_attribute(component::text, attr))
        {
            auto fgcolor = theme_.projTree.text;
            graph.string({attr.area.x, attr.area.y + 3}, compset->item_attribute().text, fgcolor);
        }
    }
//#####################################################################################################################
}

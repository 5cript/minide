#pragma once

#include "theme.hpp"
#include <nana/gui/widgets/treebox.hpp>

namespace MinIDE
{
    class ProjectTreeRenderer
        : public nana::treebox::renderer_interface
    {
        using cloneable_renderer = nana::pat::cloneable<nana::treebox::renderer_interface>;
    public:
        ProjectTreeRenderer(cloneable_renderer const& renderer);

        void begin_paint(nana::widget& w) override;
        void imbueTheme(Theme const& theme);
        void bground(graph_reference graph, compset_interface const* compset) const override;
        void expander(graph_reference graph, const compset_interface * compset) const override;
        void crook(graph_reference graph, const compset_interface * compset) const override;
        void icon(graph_reference graph, const compset_interface * compset) const override;
        void text(graph_reference graph, const compset_interface * compset) const override;

    private:
        cloneable_renderer renderer_;
        Theme theme_;
        ::nana::color fgcolor_;
    };
}

#ifndef INITANIM_H
# define INITANIM_H

# include "BVHAnimation.hpp"
# include "ImguiValues.hpp"
# include "Singleton.hpp"

class manageAnim : public IObserver {
    private:
        ImguiValues *_imgui_values;
    public:
        manageAnim(ImguiValues *imgui_values);
        ~manageAnim();

        bool	setupAnimation();
        void	detachAnimation();
        
        void    update(void *param);
};

#endif
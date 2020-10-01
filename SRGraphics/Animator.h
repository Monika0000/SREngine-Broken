#pragma once
#include "Skeleton.h"
#include "Animation.h"

#include "Component.h"

namespace SpaRcle {
	namespace Graphics {
		class Animator : public Component {
		public:
			void NextFrame();
		private:
			//Animation* 
			Skeleton* skeleton = nullptr;
		};
	}
}

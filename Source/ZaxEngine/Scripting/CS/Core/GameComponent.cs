using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace ZaxEngine
{
    public class GameComponent : Component
    {
        public GameComponent()
        {
            Internal_Create();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void Internal_Create();
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace ZaxEngine
{
    public enum EMotionType: byte
    {
        Static,                     ///< Non movable
        Kinematic,                  ///< Movable using velocities only, does not respond to forces
        Dynamic,					///< Responds to forces as a normal physics object
    }

    public class RigidBody : Component
    {
        public RigidBody()
        {
            Internal_Create();
        }

        public void SetMotionType(EMotionType type)
        {
            Internal_SetMotionType(type);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void Internal_Create();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void Internal_SetMotionType(EMotionType motionType);
    }
}
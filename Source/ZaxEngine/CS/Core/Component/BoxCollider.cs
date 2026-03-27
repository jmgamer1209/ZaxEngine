using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace ZaxEngine
{
    public class BoxCollider:Collider
    {
        public BoxCollider()
        {
            Internal_Create();
        }

        public void SetShapeScale(Vector3 scale)
        {
            Internal_SetShapeScale(scale);
        }

        public void SetSize(Vector3 size)
        {
            Internal_SetSize(size);
        }

        public void SetBoxCenter(Vector3 center)
        {
            Internal_SetBoxCenter(center);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void Internal_Create();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void Internal_SetShapeScale(Vector3 scale);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void Internal_SetSize(Vector3 size);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void Internal_SetBoxCenter(Vector3 center);
    }
}

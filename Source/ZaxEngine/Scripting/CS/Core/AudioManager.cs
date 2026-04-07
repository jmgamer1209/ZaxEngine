using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace ZaxEngine
{
    public class AudioManager
    {
        public static void Play(string path, bool loop = false)
        {
            Internal_Play(path, loop);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_Play(string path, bool loop);
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace ZaxEngine
{
    public enum TextureType:byte
    {
        Texture2D = 1, CubeMap = 2
    }
    public class Texture:ZObject
    {
        public TextureType type;
        public int ID;
        public Texture(TextureType texType, int ID) { 
            this.type = texType;
            this.ID = ID;
        }

        public static Texture Load(string path, TextureType texType = TextureType.Texture2D, bool flip_vertically = false)
        {
            return Internal_Load(path, texType, flip_vertically);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern Texture Internal_Load(string path, TextureType texType, bool flip_vertically);
    }
}

using System.Runtime.InteropServices;

namespace Lucky.Models
{
	[StructLayout(LayoutKind.Sequential, Pack = 8)]
	public struct Vec4
	{
		public float x;
		public float y;
		public float z;
		public float w;
	};
}

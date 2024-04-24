using System.Runtime.InteropServices;

namespace Lucky.Maths
{
	[StructLayout(LayoutKind.Sequential, Pack = 8)]
	public struct Vec2
	{
		public float x;
		public float y;
	};
}

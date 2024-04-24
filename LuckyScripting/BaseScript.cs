using System;
using System.Runtime.InteropServices;

namespace Lucky
{
	public abstract class BaseScript
	{
		public static Scripting Scripting = new Scripting();

		public abstract void OnUpdate(string method, float ts);

		[StructLayout(LayoutKind.Sequential, Pack = 8)]
		public struct OnUpdateArgs
		{
			public IntPtr methodNamePtr;
			public float ts;
		}

#if WEB
		// Calling from Web assembly / javascript
		public void OnUpdateInternal(object args)
		{
			var onUpdateArgs = Marshal.PtrToStructure<OnUpdateArgs>((IntPtr)Convert.ToInt32(args));
			var method = Marshal.PtrToStringAuto(onUpdateArgs.methodNamePtr);
			OnUpdate(method, onUpdateArgs.ts);
		}
#else
		// Calling from native C++
		public void OnUpdateInternal(OnUpdateArgs args)
		{
			var method = Marshal.PtrToStringAnsi(args.methodNamePtr);
			OnUpdate(method, args.ts);
		}
#endif
	}
}

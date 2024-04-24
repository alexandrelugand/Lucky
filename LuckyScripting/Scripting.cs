using System.Runtime.InteropServices;
using Lucky.Maths;

namespace Lucky
{
	public class Scripting
	{
		#region PInvoke
#if WEB
		private const string DllName = "Lucky";
#else
		private const string DllName = "__Internal";
#endif

		[DllImport(DllName)]
		public static extern void LuckyScripting_Info(string str);

		[DllImport(DllName)]
		public static extern void LuckyScripting_Warn(string str);

		[DllImport(DllName)]
		public static extern void LuckyScripting_Error(string str);

		[DllImport(DllName)]
		public static extern void LuckyScripting_LogVec2(ref Vec2 vector);

		[DllImport(DllName)]
		public static extern void LuckyScripting_LogVec3(ref Vec3 vector);

		[DllImport(DllName)]
		public static extern void LuckyScripting_LogVec4(ref Vec4 vector);

		#endregion

		#region Logging

		public void Info(string str) => LuckyScripting_Info(str);

		public void Warn(string str) => LuckyScripting_Warn(str);

		public void Error(string str) => LuckyScripting_Error(str);

		public void LogVec2(ref Vec2 vector) => LuckyScripting_LogVec2(ref vector);

		public void LogVec3(ref Vec3 vector) => LuckyScripting_LogVec3(ref vector);

		public void LogVec4(ref Vec4 vector) => LuckyScripting_LogVec4(ref vector);

		#endregion
	}
}

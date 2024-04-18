using System.Runtime.InteropServices;
using Lucky.Models;

namespace Lucky
{
	public class Scripting
	{
		#region PInvoke

#if WEB
		[DllImport("Lucky", EntryPoint = "LuckyScripting_Info")]
#else
		[DllImport("__Internal", EntryPoint = "LuckyScripting_Info")]
#endif
		public static extern void LuckyScripting_Info(string str);

#if WEB
		[DllImport("Lucky", EntryPoint = "LuckyScripting_Warn")]
#else
		[DllImport("__Internal", EntryPoint = "LuckyScripting_Warn")]
#endif
		public static extern void LuckyScripting_Warn(string str);

#if WEB
		[DllImport("Lucky", EntryPoint = "LuckyScripting_Error")]
#else
		[DllImport("__Internal", EntryPoint = "LuckyScripting_Error")]
#endif
		public static extern void LuckyScripting_Error(string str);

#if WEB
		[DllImport("Lucky", EntryPoint = "LuckyScripting_LogVec2")]
#else
		[DllImport("__Internal", EntryPoint = "LuckyScripting_LogVec2")]
#endif

		public static extern void LuckyScripting_LogVec2(ref Vec2 vector);

#if WEB
		[DllImport("Lucky", EntryPoint = "LuckyScripting_LogVec3")]
#else
		[DllImport("__Internal", EntryPoint = "LuckyScripting_LogVec3")]
#endif

		public static extern void LuckyScripting_LogVec3(ref Vec3 vector);

#if WEB
		[DllImport("Lucky", EntryPoint = "LuckyScripting_LogVec4")]
#else
		[DllImport("__Internal", EntryPoint = "LuckyScripting_LogVec4")]
#endif

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

using System.Runtime.InteropServices;

namespace Lucky
{
	public class Scripting
	{
#if WEB
		[DllImport("Lucky", EntryPoint = "LuckyScripting_Info")]
#else
		[DllImport("__Internal", EntryPoint = "LuckyScripting_Info")]
#endif
		public static extern void LuckyScripting_Info(string str);

		public void Info(string str) => LuckyScripting_Info(str);


#if WEB
		[DllImport("Lucky", EntryPoint = "LuckyScripting_Warn")]
#else
		[DllImport("__Internal", EntryPoint = "LuckyScripting_Warn")]
#endif
		public static extern void LuckyScripting_Warn(string str);

		public void Warn(string str) => LuckyScripting_Warn(str);

#if WEB
		[DllImport("Lucky", EntryPoint = "LuckyScripting_Error")]
#else
		[DllImport("__Internal", EntryPoint = "LuckyScripting_Error")]
#endif
		public static extern void LuckyScripting_Error(string str);

		public void Error(string str) => LuckyScripting_Error(str);
	}
}

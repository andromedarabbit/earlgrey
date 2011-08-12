namespace Earlgrey.EncodingTools.MultiLanguage
{
    using System;
    using System.Runtime.InteropServices;
    using System.Security;

    [StructLayout(LayoutKind.Sequential, Pack=4)]
    public struct tagSCRIPTINFO
    {
        public byte ScriptId;
        public uint uiCodePage;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst=0x30)]
        public ushort[] wszDescription;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst=0x20)]
        public ushort[] wszFixedWidthFont;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst=0x20)]
        public ushort[] wszProportionalFont;
    }
}

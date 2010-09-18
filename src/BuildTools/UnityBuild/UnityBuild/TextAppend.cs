using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    // TODO: 별로 쓸모 없을 듯
    public class TextAppend
    {
        private readonly TextReader _src;
        private readonly TextWriter _dst;

        private readonly char[] _buffer;

        public TextAppend(TextReader src, TextWriter dst)
            : this(src, dst, new char[DefaultBufferLength])
        {

        }

        public TextAppend(TextReader src, TextWriter dst, char[] buffer)
        {
            Debug.Assert(src != null);
            Debug.Assert(dst != null);

            _src = src;
            _dst = dst;

            _buffer = buffer;
        }

        // TODO: FileStream 을 직접 쓰면 인코딩이 다른 파일을 합칠 때 문제가 있을 듯
        // TODO: 게다가 아래 코드는 버그 투성이 StreamAppend 처럼 짜야 한다.
        public virtual void Merge()
        {
            _src.ReadBlock(_buffer, 0, _buffer.Length);
            _dst.Write(_buffer);
        }

        protected TextReader Reader
        {
            get { return _src; }
        }

        protected TextWriter Writer
        {
            get { return _dst; }
        }

        protected char[] InternalBuffer
        {
            get { return _buffer; }
        }

        private static long DefaultBufferLength
        {
            get { return 1024 * 1024; } 
        }
    }
}

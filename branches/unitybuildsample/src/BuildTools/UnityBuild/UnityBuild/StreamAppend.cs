using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    public class StreamAppend
    {        
        private readonly Stream _srcStream;
        private readonly Stream _dstStream;

        private readonly byte[] _buffer;

        public StreamAppend(Stream src, Stream dst)
            : this(src, dst, new byte[CalculateBufferLength(src)])
        {
        }

        public StreamAppend(Stream src, Stream dst, byte[] buffer)
        {
            Debug.Assert(src != null);            
            Debug.Assert(dst != null);
            Debug.Assert(dst.CanWrite == true);
            
            _srcStream = src;
            _dstStream = dst;

            _buffer = buffer;
        }

        // TODO: FileStream 을 직접 쓰면 인코딩이 다른 파일을 합칠 때 문제가 있을 듯
        public void Merge()
        {   
            for (long i = 0; i < _srcStream.Length; )
            {
                int bytes = (int)Math.Min(_buffer.LongLength, _srcStream.Length - i);
                i = i + bytes;

                _srcStream.Read(_buffer, 0, bytes);
                _dstStream.Write(_buffer, 0, bytes);                
            }
        }

        private static long CalculateBufferLength(Stream srcStream)
        {
            return Math.Min(srcStream.Length, 1024 * 1024);
        }
    }
}

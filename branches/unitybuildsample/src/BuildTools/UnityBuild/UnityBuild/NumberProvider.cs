using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    public sealed class NumberProvider
    {
        #region Singleton

        internal NumberProvider() // 단위테스트가 가능하게 private이 아닌 internal로 선언한다.
        {
            _currentNo = MinNo;
        }

        public static NumberProvider Instance
        {
            get
            {
                return Nested.instance;
            }
        }

        private static class Nested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static Nested()
            {
            }

            internal static readonly NumberProvider instance = new NumberProvider();
        }

        #endregion // Singleton

        #region For UnitTesting

#if DEBUG
        internal void SetNo(int number)
        {
            _currentNo = number;
        }
#endif

        #endregion

        private int _currentNo;

        private static readonly int MinNo;
        private static readonly int MaxNo;
        private static readonly string FormatString;

        static NumberProvider()
        {
            MinNo = 0;
            MaxNo = 99999;
            FormatString = "{0:00000}";
        }

        public void Next()
        {
            if (_currentNo >= MaxNo)
                throw new OverflowException();

            _currentNo++;
        }

        public int No
        {
            get
            {
                Debug.Assert(_currentNo >= MinNo && _currentNo <= MaxNo);
                  
                return _currentNo;
            }
        }

        public string NoString
        {
            get { return string.Format(FormatString, No); }
        }
    }
}

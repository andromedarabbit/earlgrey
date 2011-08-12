﻿#region License
//
// Command Line Library: TargetWrapper.cs
//
// Author:
//   Giacomo Stelluti Scala (gsscoder@ymail.com)
//
// Copyright (C) 2005 - 2010 Giacomo Stelluti Scala
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
#endregion
#region Using Directives
using System;
using System.Collections.Generic;
using System.Text;
#endregion

namespace CommandLine
{
    internal class TargetWrapper
    {
        private object _target;
        private IList<string> _valueList;
        private ValueListAttribute _vla;

        public TargetWrapper(object target)
        {
            _target = target;
            _vla = ValueListAttribute.GetAttribute(_target);
            if (IsValueListDefined)
                _valueList = ValueListAttribute.GetReference(_target);
        }

        public bool IsValueListDefined { get { return _vla != null; } }

        public bool AddValueItemIfAllowed(string item)
        {
            if (_vla.MaximumElements == 0 || _valueList.Count == _vla.MaximumElements)
                return false;

            lock (this)
            {
                _valueList.Add(item);
            }

            return true;
        }
    }
}

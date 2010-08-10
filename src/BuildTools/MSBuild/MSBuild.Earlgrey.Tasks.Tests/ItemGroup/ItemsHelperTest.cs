using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;
using MSBuild.Earlgrey.Tasks.ItemGroup;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.Tests.ItemGroup
{
    [TestFixture]
    public class ItemsHelperTest
    {
        private readonly List<string> _colors;
        private readonly IEnumerable<TaskItem> _items;

        public ItemsHelperTest()
        {
            _colors = new List<string> {"Red", "Blue", "Yellow", "Violet", "Black"};

            _items = from color in _colors
                     select new TaskItem(color)
                     ;
        }

        [Test]
        public void MinTest()
        {
            ItemsHelper helper = CreateHelperInstance();

            Assert.AreEqual("Black", _colors.Min());
            Assert.AreEqual(_colors.Min(), helper.Min.First().ItemSpec);
        }

        [Test]
        public void MaxTest()
        {
            ItemsHelper helper = CreateHelperInstance();

            Assert.AreEqual("Yellow", _colors.Max());
            Assert.AreEqual(_colors.Max(), helper.Max.First().ItemSpec);

        }
        private ItemsHelper CreateHelperInstance()
        {
            ItemsHelper helper = new ItemsHelper();
            helper.BuildEngine = new MockBuildEngine();

            helper.Items = _items.ToArray();
            return helper;
        }
    }
}

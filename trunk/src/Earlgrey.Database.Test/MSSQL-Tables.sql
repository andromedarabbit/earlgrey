USE [earlgrey_test]
GO
/****** 개체:  Table [dbo].[single_column_table]    스크립트 날짜: 12/04/2010 18:27:41 ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[single_column_table]') AND type in (N'U'))
DROP TABLE [dbo].[single_column_table]
GO
/****** 개체:  Table [dbo].[simple_table_1]    스크립트 날짜: 12/04/2010 18:27:40 ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[simple_table_1]') AND type in (N'U'))
DROP TABLE [dbo].[simple_table_1]
GO
/****** 개체:  Table [dbo].[single_column_table]    스크립트 날짜: 12/04/2010 18:27:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[single_column_table]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[single_column_table](
	[unique_key] [int] NOT NULL,
 CONSTRAINT [PK_single_column_table] PRIMARY KEY CLUSTERED 
(
	[unique_key] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
END
GO
/****** 개체:  Table [dbo].[simple_table_1]    스크립트 날짜: 12/04/2010 18:27:40 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[simple_table_1]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[simple_table_1](
	[unique_key] [int] NOT NULL,
	[name] [varchar](45) NOT NULL CONSTRAINT [DF_simple_table_1_name]  DEFAULT (''),
	[when] [datetime] NOT NULL,
	[interval] [datetime] NOT NULL,
 CONSTRAINT [PK_simple_table_1] PRIMARY KEY CLUSTERED 
(
	[unique_key] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_PADDING OFF
GO


-- Populating records
---- simple_table
INSERT INTO dbo.simple_table_1 VALUES (0,'가','2010-02-27 21:49:43','1899-12-30 00:00:00')
GO
INSERT INTO dbo.simple_table_1 VALUES (1,'나','2011-02-27 21:49:44','1900-01-01 21:49:43')
GO
INSERT INTO dbo.simple_table_1 VALUES(2,'다','2012-02-27 21:49:45','1900-01-04 01:09:03')
GO

---- single_column)_table
-- Populating records
INSERT INTO dbo.single_column_table VALUES (100)
GO

USE [master]
GO
/****** Object:  Database [earlgrey_test]    Script Date: 06/13/2011 10:23:34 ******/
IF NOT EXISTS (SELECT name FROM sys.databases WHERE name = N'earlgrey_test')
BEGIN
CREATE DATABASE [earlgrey_test]
END
GO

ALTER DATABASE [earlgrey_test] SET ANSI_NULL_DEFAULT OFF
GO
ALTER DATABASE [earlgrey_test] SET ANSI_NULLS OFF
GO
ALTER DATABASE [earlgrey_test] SET ANSI_PADDING OFF
GO
ALTER DATABASE [earlgrey_test] SET ANSI_WARNINGS OFF
GO
ALTER DATABASE [earlgrey_test] SET ARITHABORT OFF
GO
ALTER DATABASE [earlgrey_test] SET AUTO_CLOSE ON
GO
ALTER DATABASE [earlgrey_test] SET AUTO_CREATE_STATISTICS ON
GO
ALTER DATABASE [earlgrey_test] SET AUTO_SHRINK OFF
GO
ALTER DATABASE [earlgrey_test] SET AUTO_UPDATE_STATISTICS ON
GO
ALTER DATABASE [earlgrey_test] SET CURSOR_CLOSE_ON_COMMIT OFF
GO
ALTER DATABASE [earlgrey_test] SET CURSOR_DEFAULT  GLOBAL
GO
ALTER DATABASE [earlgrey_test] SET CONCAT_NULL_YIELDS_NULL OFF
GO
ALTER DATABASE [earlgrey_test] SET NUMERIC_ROUNDABORT OFF
GO
ALTER DATABASE [earlgrey_test] SET QUOTED_IDENTIFIER OFF
GO
ALTER DATABASE [earlgrey_test] SET RECURSIVE_TRIGGERS OFF
GO
ALTER DATABASE [earlgrey_test] SET  ENABLE_BROKER
GO
ALTER DATABASE [earlgrey_test] SET AUTO_UPDATE_STATISTICS_ASYNC OFF
GO
ALTER DATABASE [earlgrey_test] SET DATE_CORRELATION_OPTIMIZATION OFF
GO
ALTER DATABASE [earlgrey_test] SET TRUSTWORTHY OFF
GO
ALTER DATABASE [earlgrey_test] SET ALLOW_SNAPSHOT_ISOLATION OFF
GO
ALTER DATABASE [earlgrey_test] SET PARAMETERIZATION SIMPLE
GO
ALTER DATABASE [earlgrey_test] SET READ_COMMITTED_SNAPSHOT OFF
GO
ALTER DATABASE [earlgrey_test] SET  READ_WRITE
GO
ALTER DATABASE [earlgrey_test] SET RECOVERY SIMPLE
GO
ALTER DATABASE [earlgrey_test] SET  MULTI_USER
GO
ALTER DATABASE [earlgrey_test] SET PAGE_VERIFY CHECKSUM
GO
ALTER DATABASE [earlgrey_test] SET DB_CHAINING OFF
GO
USE [earlgrey_test]
GO
/****** 개체:  StoredProcedure [dbo].[SimpleProcedure]    스크립트 날짜: 04/01/2011 22:18:27 ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[SimpleProcedure]') AND type in (N'P', N'PC'))
DROP PROCEDURE [dbo].[SimpleProcedure]
GO
/****** 개체:  Table [dbo].[single_column_table]    스크립트 날짜: 04/01/2011 22:18:27 ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[single_column_table]') AND type in (N'U'))
DROP TABLE [dbo].[single_column_table]
GO
/****** 개체:  Table [dbo].[simple_table_1]    스크립트 날짜: 04/01/2011 22:18:27 ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[simple_table_1]') AND type in (N'U'))
DROP TABLE [dbo].[simple_table_1]
GO
/****** 개체:  Table [dbo].[simple_table_1]    스크립트 날짜: 04/01/2011 22:18:27 ******/
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
	[just_number] [int] NOT NULL,
 CONSTRAINT [PK_simple_table_1] PRIMARY KEY CLUSTERED 
(
	[unique_key] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
END
GO
SET ANSI_PADDING OFF
GO
IF NOT EXISTS (SELECT * FROM sys.indexes WHERE object_id = OBJECT_ID(N'[dbo].[simple_table_1]') AND name = N'IX_simple_table_1')
CREATE NONCLUSTERED INDEX [IX_simple_table_1] ON [dbo].[simple_table_1] 
(
	[unique_key] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[single_column_table]    Script Date: 06/13/2011 10:23:35 ******/
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
/****** Object:  Trigger [SimpleTrigger]    Script Date: 06/13/2011 10:23:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.triggers WHERE object_id = OBJECT_ID(N'[dbo].[SimpleTrigger]'))
EXEC dbo.sp_executesql @statement = N'CREATE TRIGGER [dbo].[SimpleTrigger] 
   ON  [dbo].[simple_table_1] 
   AFTER INSERT,DELETE,UPDATE
AS 
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

    -- Insert statements for trigger here

END
'
GO
/****** Object:  StoredProcedure [dbo].[SimpleProcedure]    Script Date: 06/13/2011 10:23:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[SimpleProcedure]') AND type in (N'P', N'PC'))
BEGIN
EXEC dbo.sp_executesql @statement = N'CREATE PROCEDURE [dbo].[SimpleProcedure] 
	-- Add the parameters for the stored procedure here
	@Count int = 0
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

	-- Insert statements for procedure here
	SET ROWCOUNT @Count
	SELECT * FROM dbo.simple_table_1
	SET ROWCOUNT 0 
END
' 
END
GO
/****** Object:  Default [DF_simple_table_1_name]    Script Date: 06/13/2011 10:23:35 ******/
IF Not EXISTS (SELECT * FROM sys.default_constraints WHERE object_id = OBJECT_ID(N'[dbo].[DF_simple_table_1_name]') AND parent_object_id = OBJECT_ID(N'[dbo].[simple_table_1]'))
Begin
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[DF_simple_table_1_name]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[simple_table_1] ADD  CONSTRAINT [DF_simple_table_1_name]  DEFAULT ('') FOR [name]
END


End
GO
/****** Object:  Default [DF_simple_table_1_just_number]    Script Date: 06/13/2011 10:23:35 ******/
IF Not EXISTS (SELECT * FROM sys.default_constraints WHERE object_id = OBJECT_ID(N'[dbo].[DF_simple_table_1_just_number]') AND parent_object_id = OBJECT_ID(N'[dbo].[simple_table_1]'))
Begin
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[DF_simple_table_1_just_number]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[simple_table_1] ADD  CONSTRAINT [DF_simple_table_1_just_number]  DEFAULT ((9)) FOR [just_number]
END


End
GO

-- populate records
INSERT [dbo].[simple_table_1] ([unique_key], [name], [when], [interval]) VALUES (0, N'가', CAST(0x00009D2A0167B974 AS DateTime), CAST(0xFFFFFFFE00000000 AS DateTime))
INSERT [dbo].[simple_table_1] ([unique_key], [name], [when], [interval]) VALUES (1, N'나', CAST(0x00009E970167BAA0 AS DateTime), CAST(0x000000000167B974 AS DateTime))
INSERT [dbo].[simple_table_1] ([unique_key], [name], [when], [interval]) VALUES (2, N'다', CAST(0x0000A0040167BBCC AS DateTime), CAST(0x000000030012F714 AS DateTime))
GO

INSERT [dbo].[single_column_table] ([unique_key]) VALUES (100)
GO
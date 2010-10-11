#include "stdafx.h"
#include "EventProducer.h"

using namespace Earlgrey;

class DummyEventProducerContainer
{

public:
	EventProducer<void()> ep0;
	EventProducer<void(int)> ep1;
	EventProducer<void(int, _txstring)> ep2;
};

class EventProducerTestFixture : public ::testing::Test
{
public:
	void SetUp()
	{
		epContainer.ep0.AddListener( this, std::tr1::bind( &EventProducerTestFixture::Listen0, this ) );
		epContainer.ep1.AddListener( this, std::tr1::bind( &EventProducerTestFixture::Listen1, this, std::tr1::placeholders::_1 ) );
		epContainer.ep2.AddListener( this, std::tr1::bind( &EventProducerTestFixture::Listen2, this, std::tr1::placeholders::_1, std::tr1::placeholders::_2 ) );

		count0 = 0;
		sum1 = 0;
		sum2 = 0;
	}

	void TearDown()
	{
	}

	void Listen0()
	{
		count0++;
	}

	void Listen1(int a)
	{
		sum1 += a;
	}

	void Listen2(int a, _txstring s)
	{
		sum2 += a;
		text.append( s );
	}

	void RemoveListenerEp0()
	{
		epContainer.ep0.RemoveListener( this );
	}

	void RemoveListenerEp1()
	{
		epContainer.ep1.RemoveListener( this );
	}

	void RemoveListenerEp2()
	{
		epContainer.ep2.RemoveListener( this );
	}

protected:
	DummyEventProducerContainer epContainer;
	int count0;
	int sum1, sum2;
	_txstring text;
};


TEST_F(EventProducerTestFixture, FireTest)
{
	epContainer.ep0.Fire();
	epContainer.ep0.Fire();
	epContainer.ep0.Fire();

	EXPECT_EQ( 3, count0 );

	epContainer.ep0.Fire();
	epContainer.ep0.Fire();

	EXPECT_EQ( 5, count0 );

	RemoveListenerEp0();

	epContainer.ep0.Fire();

	EXPECT_EQ( 5, count0 );

	epContainer.ep1.Fire( 10 );
	epContainer.ep1.Fire( 1 );

	EXPECT_EQ( 11, sum1 );

	epContainer.ep2.Fire( 1, _T("TEST") );
	epContainer.ep2.Fire( 1, _T("GOGO") );

	EXPECT_TRUE( text == _T("TESTGOGO") );


}